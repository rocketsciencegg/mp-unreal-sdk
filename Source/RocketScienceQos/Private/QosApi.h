 /*
 * #####################################################################################
 *  Unity Gaming Services Plugin for Unreal Engine Copyright © 2025 Unity Technologies.
 * #####################################################################################
 */

#pragma once

#include "Models/RSGetSortedQosResultsResponse.h"
#include "Utils/ServicesUtils.h"
#include "RSQosSubsystem.h"
#include "IQosApi.h"
#include "QosRunnable.h"

namespace RocketScienceQos
{
        class FQosApi
        {
        public:
            FQosApi() = default;
		    FQosApi(TUniquePtr<IQosApi> ClientApi);

            /**
            * @brief Gets sorted QoS measurements for the specified fleet and regions.
			* @param FleetId            The fleet ID to query for QoS.
            * @param Regions            The regions to query for QoS.
            * @param ResponseHandler	The response callback to use once CreateTicket has finished.
            */
            void GetSortedQosResultsAsync(FString FleetId, TArray<FString> Regions, Unity::Services::Core::THandler<FRSGetSortedQosResultsResponse> ResponseHandler);

        private:

            // Response handler
            void ListServersHandleResponse(const ListServersResponse& Response);

            /**
            * @brief Gets servers used to determine connection quality between a client and a given server.
            */
            void ListServers(FString FleetId);

            /**
            * @brief Filters the QoS servers based on service and region
            */
            void FilterQosServers();

            /**
			* @brief Sends UDP packets to the QoS servers a set number of times to measure latency and packet loss.
            */
            void MeasureQosServers();

            /**
            * @brief Sorts the qos measurments and returns an array of QoS results.
            * @return					The array of sorted QoS results.
            */
            TArray<FRSQosResult> GetResultsSortedByLatency();

            /**
			* @brief To be called by QosRunner worker threads before they finish - schedules the saving of a QoS result back on the game thread.
            * @param Server            The server that has finished being measured.
			* @param AverageLatencyMs  The average latency to the server in milliseconds.
			* @param PacketsLost       The number of packets lost.
            */
			void SaveQosResult(const FRSQosServer& Server, uint64 AverageLatencyMs, int PacketsLost);

            /**
			* @brief Clean up Qos results retrieval process and call the response handler.
			* @param bWasSuccessful    Whether the QoS retrieval process was successful.
			* @param ErrorMessage      An error message to log if the process was not successful.
            */
            void FinaliseQosResults(const bool bWasSuccessful, const FString& ErrorMessage = "");

        private:

            TUniquePtr<IQosApi> ClientApi;
            Unity::Services::Core::THandler<FRSGetSortedQosResultsResponse> GetSortedQosResultsResponseHandler;

            /** Array of QoS servers returned from the ListServers call. */
            TArray<OpenAPIQosServer> ReturnedQosServers = {};

            /** Array of QoS servers to be used for QoS measurement. */
            TArray<FRSQosServer> QosServersToMeasure = {};

            /** Array of QoS results which will be returned to the client after processing. */
            TArray<FRSQosResult> QosResults = {};

            uint32 NumberOfQosResults = 0;

            /** Number of packets to send to each QoS server. */
            const uint32 NumberOfPacketsPerQosServer = 10;

            /** Bool used to prevent multiple QoS requests happening at the same time. */
            bool RetrievingQosResults = false;

            TArray<FString> QosRegionsToSort;

            TArray<TSharedPtr<FQosRunnable>> QosRunnables;
	};
}

