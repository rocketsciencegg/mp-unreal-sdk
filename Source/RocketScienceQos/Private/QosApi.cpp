 /*
 * #####################################################################################
 *  Unity Gaming Services Plugin for Unreal Engine Copyright © 2025 Unity Technologies.
 * #####################################################################################
 */

#include "QosApi.h"
#include "RSQosModule.h"
#include "Kismet/GameplayStatics.h"
#include "Templates/UniquePtr.h"

namespace RocketScienceQos
{
	
		FQosApi::FQosApi(TUniquePtr<IQosApi> ClientApi)
			: ClientApi(MoveTemp(ClientApi))
		{
		}

		void FQosApi::GetSortedQosResultsAsync(FString FleetId, TArray<FString> Regions, Unity::Services::Core::THandler<FRSGetSortedQosResultsResponse> ResponseHandler)
		{
			// Check if we are already attempting to retrieve the sorted qos results
			if (RetrievingQosResults)
			{
				UE_LOG(LogRSQos, Error, TEXT("Failed to begin process of retrieving sorted qos results as qos results are already in the process of being retrieved!"));
				return;
			}

			RetrievingQosResults = true;
			GetSortedQosResultsResponseHandler = ResponseHandler;

			ListServers(FleetId); // this calls the next steps in its response handler
		}

		void FQosApi::ListServers(FString FleetId)
		{
			ListServersRequest Request;
			Request.Fleetid = FleetId;

			FListServersDelegate GetServersDelegate;
			GetServersDelegate.BindRaw(this, &FQosApi::ListServersHandleResponse);

			ClientApi->ListServers(Request, GetServersDelegate);
		}

		void FQosApi::ListServersHandleResponse(const ListServersResponse& Response)
		{		
			if (Response.IsSuccessful() && Response.Content.Servers.IsSet())
			{
				ReturnedQosServers = Response.Content.Servers.GetValue();
				FilterQosServers();
				MeasureQosServers();
				return;
			}

			FinaliseQosResults(false, FString::Printf(TEXT("Invalid server response when trying to get servers: %s"), *Response.GetResponseString()));
			return;
		}

		void FQosApi::FilterQosServers()
		{
			QosServersToMeasure.Empty();

			for (auto server : ReturnedQosServers)
			{
				FRSQosServer qosServer;

				// TODO (Jac Griffiths): verify new GUIDs for region ID work correctly here
				if (QosRegionsToSort.Num() > 0 && !QosRegionsToSort.Contains(server.Regionid.ToString()))
				{
					// If we want to only return the specifically requested regions, continue if this server is not in one of those regions
					continue;
				}

				qosServer.Region = server.Regionid.ToString();
				qosServer.Endpoints = { server.Ipv4 };
				qosServer.Ip = server.Ipv4;
				qosServer.Port = FString::FromInt(server.Port);

				QosServersToMeasure.Add(qosServer);
			}
		}

	
		void FQosApi::MeasureQosServers()
		{
			if (QosServersToMeasure.Num() == 0)
			{
				FinaliseQosResults(false, TEXT("Getting servers recieved a successful response, but no servers were found in response body!"));
				return;
			}

			QosResults.Empty();
			NumberOfQosResults = 0;

			UE_LOG(LogRSQos, Log, TEXT("Beginning QoS measurements for %d servers."), QosServersToMeasure.Num());
			for (auto Server : QosServersToMeasure)
			{
				TSharedPtr<FQosRunnable> QosRunnable = MakeShared<FQosRunnable>(Server, NumberOfPacketsPerQosServer);
				QosRunnable->OnQosComplete.BindRaw(this, &FQosApi::SaveQosResult);
				QosRunnables.Add(QosRunnable);
				UE_LOG(LogRSQos, Verbose, TEXT("QoS Runnable Created for %s:%s"), *Server.Ip, *Server.Port);
			}
		}

		TArray<FRSQosResult> FQosApi::GetResultsSortedByLatency()
		{
			QosResults.Sort([](const FRSQosResult& Lhs, const FRSQosResult& Rhs) -> bool {
				// sort by lowest latency
				if (Lhs.Latency < Rhs.Latency) { return true; }
				// if latency is equivalant, sort by packet loss
				if (Lhs.Latency == Rhs.Latency && Lhs.PacketLoss < Rhs.PacketLoss) { return true; }

				return false;
				});

			return QosResults;
		}

		void FQosApi::SaveQosResult(const FRSQosServer& Server, uint64 AverageLatencyMs, int PacketsLost)
		{
			// Called from worker threads, but accessing shared resources and eventually Unreal callbacks, so queue this to be ran on the game thread
			AsyncTask(ENamedThreads::GameThread, [this, Server, AverageLatencyMs, PacketsLost]()
			{
				FString RegionId = Server.Region;
				FRSQosResult* FoundResult = QosResults.FindByPredicate([&, RegionId](FRSQosResult qosResult) { return qosResult.RegionId == RegionId; });

				// If we didn't find a result, we haven't added one to the array yet for this region, so we create an entry for it.
				if (FoundResult == nullptr)
				{
					FRSQosResult NewResult;
					NewResult.RegionId = RegionId;
					int idx = QosResults.Add(NewResult);
					FoundResult = &QosResults[idx];
				}

				FoundResult->AggregateLatency += AverageLatencyMs;
				FoundResult->AggregatePacketsLost += PacketsLost;
				FoundResult->NumResultsForRegion++;

				NumberOfQosResults++;
				int NumberOfExpectedResults = QosServersToMeasure.Num();
				UE_LOG(LogRSQos, Verbose, TEXT("Saved Qos Results: %d/%d"), NumberOfQosResults, NumberOfExpectedResults);

				if (NumberOfQosResults == NumberOfExpectedResults)
				{
					FinaliseQosResults(true);	
				}
			});
		}

		void FQosApi::FinaliseQosResults(const bool bWasSuccessful, const FString& ErrorMessage)
		{
			if(ErrorMessage.Len() > 0)
			{
				UE_LOG(LogRSQos, Error, TEXT("Finalising QoS results with error: %s"), *ErrorMessage);
			}
			else {
				UE_LOG(LogRSQos, Log, TEXT("Finished gathering QoS results."));
			}

			for(const auto& Runnable : QosRunnables)
			{
				Runnable->Stop();
			}

			QosRunnables.Empty();
			
			for (auto& QosResult : QosResults) 
			{
				QosResult.Latency = QosResult.AggregateLatency / QosResult.NumResultsForRegion;
				QosResult.PacketLoss = QosResult.AggregatePacketsLost / QosResult.NumResultsForRegion;
				QosResult.PacketLossPercentage = QosResult.PacketLoss / (float)(NumberOfPacketsPerQosServer);

				UE_LOG(LogRSQos, Log, TEXT("Region: %s, Latency: %d ms, Packet Loss: %d packets (%.2f%%)"), *QosResult.RegionId, QosResult.Latency, QosResult.PacketLoss, QosResult.PacketLossPercentage * 100.f);
			}

			FRSGetSortedQosResultsResponse Response;
			Response.bWasSuccessful = bWasSuccessful;
			Response.ErrorMessage = ErrorMessage;
			Response.QosResults = GetResultsSortedByLatency();
			GetSortedQosResultsResponseHandler.ExecuteIfBound(Response);
			RetrievingQosResults = false;
		}
}
