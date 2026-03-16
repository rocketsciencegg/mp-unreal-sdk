 /*
 * #####################################################################################
 *  Rocket Science Multiplay Plugin for Unreal Engine Copyright © 2026 Rocket Science Group
 * #####################################################################################
 */

#include "QosRunnable.h"
#include "CoreMinimal.h"
#include "RSQosModule.h"
#include "Sockets.h"
#include "SocketSubsystem.h"
#include "Networking.h"
#include "IPAddress.h"
#include "QosRequestPacket.h"
#include "Misc/DefaultValueHelper.h"

namespace RocketScienceQos
{
	bool FQosRunnable::SetupSocket(TSharedRef<FInternetAddr> Address)
	{
		Socket = FUdpSocketBuilder(TEXT("QosUDPSocket"))
			.AsNonBlocking();

		bool bIsValid;
		Address->SetIp(*Server.Ip, bIsValid);
		
		if(!bIsValid)
		{
			UE_LOG(LogRSQos, Error, TEXT("Failed to set up socket for QoS runnable as IP address %s is invalid!"), *Server.Ip);
			return false;
		}

		int32 Port = 0;
		if(!FDefaultValueHelper::ParseInt(*Server.Port, Port))
		{
			UE_LOG(LogRSQos, Error, TEXT("Failed to set up socket for QoS runnable as port %s is invalid!"), *Server.Port);
			return false;
		}

		Address->SetPort(Port);

		return true;
	}

	void FQosRunnable::SendPacket(TSharedRef<FInternetAddr> Address, uint8 Sequence)
	{
		const FString PacketTitle = "Qos request";
		FQosRequestPacket QosPacket(PacketTitle, Sequence);
		QosPacket.SetTimestamp(GetCurrentTimestampMs());
		QosPacket.SetIdentifier(FMath::RandRange(0, UINT16_MAX));

		const auto Data = QosPacket.ToBytes();
		int32 BytesSent = 0;
		Socket->SendTo(Data.GetData(), Data.Num(), BytesSent, *Address);
		UE_LOG(LogRSQos, VeryVerbose, TEXT("Sent QoS packet to %s:%s. Sequence: %d, Identifier: %d, Timestamp: %d"), *Server.Ip, *Server.Port, Sequence, QosPacket.GetIdentifier(), QosPacket.GetTimestamp());
	}

	bool FQosRunnable::TryReceivePacket(TSharedRef<FInternetAddr> Address, uint64 RemainingWaitTimeMs, FQosResponsePacket& OutResponsePacket)
	{
		if (Socket->Wait(ESocketWaitConditions::WaitForRead, FTimespan::FromMilliseconds(RemainingWaitTimeMs)))
		{
			uint8 RecvBuffer[2048];
			int32 BytesRead = 0;
			Socket->RecvFrom(RecvBuffer, sizeof(RecvBuffer), BytesRead, *Address);
			TArray<uint8> ReceivedData(RecvBuffer, BytesRead);
			
			FQosResponsePacket ResponsePacket;
			ResponsePacket.SetReceivedTimestamp(GetCurrentTimestampMs());
			ResponsePacket.FromBytes(ReceivedData);

			OutResponsePacket = ResponsePacket;
			return true;
		}
		else {
			return false;
		}
	}

	uint64 FQosRunnable::GetCurrentTimestampMs() const
	{
		return FDateTime::UtcNow().GetTicks() / ETimespan::TicksPerMillisecond;
	}

	FQosRunnable::FQosRunnable(const FRSQosServer& InServer, const int InPacketsToSend)
	{
		Server = InServer;
		PacketsToSend = InPacketsToSend;
		FString ThreadName = FString::Printf(TEXT("QosRunnable_%s:%s"), *Server.Ip, *Server.Port);
		Thread = FRunnableThread::Create(this, *ThreadName);
	}

	FQosRunnable::~FQosRunnable()
	{
		if (Thread != nullptr)
		{
			Thread->Kill(true);
			delete Thread;
		}
	}

	uint32 FQosRunnable::Run()
	{
		UE_LOG(LogRSQos, Verbose, TEXT("QoS Runnable Starting for %s:%s at %d"), *Server.Ip, *Server.Port, GetCurrentTimestampMs());

		bStopThread = false;

		TSharedRef<FInternetAddr> Address = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();

		if (!SetupSocket(Address)) {
			OnQosComplete.ExecuteIfBound(Server, FailureLatencyResult, FailurePacketLossResult);
			return 1;
		}

		for (int i = 0; i < PacketsToSend; i++) {
			SendPacket(Address, i);
			FPlatformProcess::Sleep(0.01f); // Sleep to prevent outgoing packets being batched
		}

		int PacketsReceived = 0;
		uint64 RemainingWaitMs = MaxWaitMs;
		uint64 LastWaitStartTimestamp = GetCurrentTimestampMs();
		uint64 TotalLatency = 0;

		UE_LOG(LogRSQos, Log, TEXT("Started listening for return packets at: %d"), LastWaitStartTimestamp);

		while(PacketsReceived < PacketsToSend && !bStopThread)
		{
			FQosResponsePacket ResponsePacket;
			if (TryReceivePacket(Address, RemainingWaitMs, ResponsePacket))
			{
				uint64 ReceivedTimestamp = ResponsePacket.GetReceivedTimestamp();

				uint64 Latency = ReceivedTimestamp - ResponsePacket.GetSentTimestamp();
				TotalLatency += Latency;

				UE_LOG(LogRSQos, VeryVerbose, TEXT("Received QoS packet %d from %s:%s."), PacketsReceived, *Server.Ip, *Server.Port);
				UE_LOG(LogRSQos, VeryVerbose, TEXT("Sequence: %d, Identifier: %d, SentTimestamp: %d, ReceivedTimestamp: %d"), ResponsePacket.GetSequence(), ResponsePacket.GetIdentifier(), ResponsePacket.GetSentTimestamp(), ReceivedTimestamp);
				UE_LOG(LogRSQos, VeryVerbose, TEXT("Calculated latency: %d ms"), Latency);

				PacketsReceived++;
				RemainingWaitMs -= (ReceivedTimestamp - LastWaitStartTimestamp);
				LastWaitStartTimestamp = ReceivedTimestamp;
			}
			else {
				UE_LOG(LogRSQos, Warning, TEXT("QoS Runnable Timed Out at %d"), GetCurrentTimestampMs());
				break;
			}
		}

		UE_LOG(LogRSQos, Verbose, TEXT("QoS Runnable finished for %s:%s with %d/%d packets received."), *Server.Ip, *Server.Port, PacketsReceived, PacketsToSend);

		uint64 AverageLatencyMs = PacketsReceived > 0 ? TotalLatency / PacketsReceived : FailureLatencyResult;
		int PacketsLost = PacketsToSend - PacketsReceived;

		UE_LOG(LogRSQos, Verbose, TEXT("Average Latency: %d ms, Packets Lost: %d"), AverageLatencyMs, PacketsLost);

		OnQosComplete.ExecuteIfBound(Server, AverageLatencyMs, PacketsLost);

		return 0;
	}

	void FQosRunnable::Stop()
	{
		bStopThread = true;
	}

	void FQosRunnable::Exit()
	{
	}
}

