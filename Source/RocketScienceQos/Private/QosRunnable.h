#pragma once

#include "CoreMinimal.h"
#include "Models/RSQosServer.h"
#include "QosResponsePacket.h"

DECLARE_DELEGATE_ThreeParams(FOnQosComplete, const FRSQosServer& /*Server*/, uint64 /*Average Latency Ms*/, int /*Packets Lost*/);

namespace RocketScienceQos 
{	

	class FQosRunnable : FRunnable
	{

	private:

		static constexpr int32 FailureLatencyResult = INT32_MAX;
		static constexpr int FailurePacketLossResult = INT_MAX;

		const int MaxWaitMs = 500;
		int PacketsToSend;

		FRunnableThread* Thread = nullptr;
		bool bStopThread = false;
		FRSQosServer Server;
		FSocket* Socket;

		bool SetupSocket(TSharedRef<FInternetAddr> Address);
		void SendPacket(TSharedRef<FInternetAddr>, uint8 Sequence);
		bool TryReceivePacket(TSharedRef<FInternetAddr> Address, uint64 RemainingWaitTimeMs, FQosResponsePacket& OutResponsePacket);
		uint64 GetCurrentTimestampMs() const;


	public:

		FQosRunnable(const FRSQosServer& InServer, const int InPacketsToSend);
		virtual ~FQosRunnable();

		virtual uint32 Run() override;
		virtual void Stop() override;
		virtual void Exit() override;

		FOnQosComplete OnQosComplete;
	};
}