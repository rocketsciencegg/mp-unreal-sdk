/*
 * #####################################################################################
 *  Rocket Science Multiplay Plugin for Unreal Engine Copyright © 2026 Rocket Science Group
 * #####################################################################################
 */

#pragma once

#include "CoreMinimal.h"
#include "Dom/JsonObject.h"
#include "MultiplayCentrifugeForwardDeclarations.h"

class IWebSocket;

namespace RocketScienceMultiplaySDK
{
	enum class EConnectionStatus
	{
		Disconnected,
		Connected,
		Connecting,
		Disconnecting,
	};

	struct FRequest
	{
		uint32 Id;
		EMethodType Method;
	};

	class FCentrifugeClient
	{
	public:
		// Id 0 is reserved for Push messages.
		static constexpr uint32 kPushId = 0;

		// Ids for Command messages start at 1.
		static constexpr uint32 kInitialMsgId = 1;

	public:
		FCentrifugeClient(FString Url);
		~FCentrifugeClient();

		void Disconnect();

	public:
		// Command Messages
		void Connect(const FConnectRequest& Request);
		void Subscribe(const FSubscribeRequest& Request);
		void Unsubscribe(const FUnsubscribeRequest& Request);
		void Publish(const FPublishRequest& Request);
		void Presence(const FPresenceRequest& Request);
		void PresenceStats(const FPresenceStatsRequest& Request);
		void History(const FHistoryRequest& Request);
		void Ping(const FPingRequest& Request);
		void Send(const FSendRequest& Request);
		void Rpc(const FRpcRequest& Request);
		void Refresh(const FRefreshRequest& Request);
		void SubRefresh(const FSubRefreshRequest& Request);

	public:
		// Reply Messages
		DECLARE_EVENT_OneParam(FCentrifugeClient, FConnectReplyEvent, const FConnectResult&)
		FConnectReplyEvent& OnConnectReply() { return ConnectReply; }

		DECLARE_EVENT_OneParam(FCentrifugeClient, FSubscribeReplyEvent, const FSubscribeResult&)
		FSubscribeReplyEvent& OnSubscribeReply() { return SubscribeReply; }

		DECLARE_EVENT_OneParam(FCentrifugeClient, FUnsubscribeReplyEvent, const FUnsubscribeResult&)
		FUnsubscribeReplyEvent& OnUnsubscribeReply() { return UnsubscribeReply; }

		DECLARE_EVENT_OneParam(FCentrifugeClient, FPublishReplyEvent, const FPublishResult&)
		FPublishReplyEvent& OnPublishReply() { return PublishReply; }

		DECLARE_EVENT_OneParam(FCentrifugeClient, FPresenceReplyEvent, const FPresenceResult&)
		FPresenceReplyEvent& OnPresenceReply() { return PresenceReply; }

		DECLARE_EVENT_OneParam(FCentrifugeClient, FPresenceStatsReplyEvent, const FPresenceStatsResult&)
		FPresenceStatsReplyEvent& OnPresenceStatsReply() { return PresenceStatsReply; }

		DECLARE_EVENT_OneParam(FCentrifugeClient, FHistoryReplyEvent, const FHistoryResult&)
		FHistoryReplyEvent& OnHistoryReply() { return HistoryReply; }

		DECLARE_EVENT_OneParam(FCentrifugeClient, FPingReplyEvent, const FPingResult&)
		FPingReplyEvent& OnPingReply() { return PingReply; }

		DECLARE_EVENT_OneParam(FCentrifugeClient, FRpcReplyEvent, const FRpcResult&)
		FRpcReplyEvent& OnRpcReply() { return RpcReply; }

		DECLARE_EVENT_OneParam(FCentrifugeClient, FRefreshReplyEvent, const FRefreshResult&)
		FRefreshReplyEvent& OnRefreshReply() { return RefreshReply; }

		DECLARE_EVENT_OneParam(FCentrifugeClient, FSubRefreshReplyEvent, const FSubRefreshResult&)
		FSubRefreshReplyEvent& OnSubRefreshReply() { return SubRefreshReply; }

	public:
		// Push Messages
		DECLARE_EVENT_OneParam(FCentrifugeClient, FPublicationPushEvent, const FPublication&)
		FPublicationPushEvent& OnPublicationPush() { return PublicationPush; }

		DECLARE_EVENT_OneParam(FCentrifugeClient, FJoinPushEvent, const FJoin&)
		FJoinPushEvent& OnJoinPush() { return JoinPush; }

		DECLARE_EVENT_OneParam(FCentrifugeClient, FLeavePushEvent, const FLeave&)
		FLeavePushEvent& OnLeavePush() { return LeavePush; }

		DECLARE_EVENT_OneParam(FCentrifugeClient, FUnsubscribePushEvent, const FUnsubscribe&)
		FUnsubscribePushEvent& OnUnsubscribePush() { return UnsubscribePush; }

		DECLARE_EVENT_OneParam(FCentrifugeClient, FMessagePushEvent, const FMessage&)
		FMessagePushEvent& OnMessagePush() { return MessagePush; }

		DECLARE_EVENT_OneParam(FCentrifugeClient, FSubscribePushEvent, const FSubscribe&)
		FSubscribePushEvent& OnSubscribePush() { return SubscribePush; }

		DECLARE_EVENT_OneParam(FCentrifugeClient, FConnectPushEvent, const FConnect&)
		FConnectPushEvent& OnConnectPush() { return ConnectPush; }

		DECLARE_EVENT_OneParam(FCentrifugeClient, FDisconnectPushEvent, const FDisconnect&)
		FDisconnectPushEvent& OnDisconnectPush() { return DisconnectPush; }

		DECLARE_EVENT_OneParam(FCentrifugeClient, FRefreshPushEvent, const FRefresh&)
		FRefreshPushEvent& OnRefreshPush() { return RefreshPush; }

	public:
		//State events
		DECLARE_EVENT_OneParam(FCentrifugeClient, FConnectionStatusChangedEvent, const EConnectionStatus&)
		FConnectionStatusChangedEvent& OnConnectionStatusChanged() { return ConnectionStatusChanged; }

	private:
		// IWebSocket
		void OnConnected();
		void OnConnectionError(const FString& Error);
		void OnClosed(int32 StatusCode, const FString& Reason, bool bWasClean);
		void OnMessage(const FString& MessageString);
		void OnMessageSent(const FString& MessageString);

	private:
		void ChangeConnectionStatus(EConnectionStatus NewStatus);

		uint32 GetNextMessageId();

		template <typename T>
		void SendRequest(const T& Request);

		bool TryGetError(const TSharedPtr<FJsonObject>& JsonObject);
		bool TryGetReply(const TSharedPtr<FJsonObject>& JsonObject);
		bool TryGetPush(const TSharedPtr<FJsonObject>& JsonObject);

		void ParseCentrifugeMessages(const FString& MessageString);
		void ParseCentrifugeMessage(const FString& MessageString);

	private:
		// Reply Messages
		FConnectReplyEvent ConnectReply;
		FSubscribeReplyEvent SubscribeReply;
		FUnsubscribeReplyEvent UnsubscribeReply;
		FPublishReplyEvent PublishReply;
		FPresenceReplyEvent PresenceReply;
		FPresenceStatsReplyEvent PresenceStatsReply;
		FHistoryReplyEvent HistoryReply;
		FPingReplyEvent PingReply;
		FRpcReplyEvent RpcReply;
		FRefreshReplyEvent RefreshReply;
		FSubRefreshReplyEvent SubRefreshReply;
		
	private:
		// Push Messages
		FPublicationPushEvent PublicationPush;
		FJoinPushEvent JoinPush;
		FLeavePushEvent LeavePush;
		FUnsubscribePushEvent UnsubscribePush;
		FMessagePushEvent MessagePush;
		FSubscribePushEvent SubscribePush;
		FConnectPushEvent ConnectPush;
		FDisconnectPushEvent DisconnectPush;
		FRefreshPushEvent RefreshPush;

	private:
		//State Events
		FConnectionStatusChangedEvent ConnectionStatusChanged;

	private:
		FString Url;
		uint32 Id;
		EConnectionStatus Status;
		TSharedPtr<IWebSocket> WebSocket;
		TMap<uint32, FRequest> Requests;
	};
} // namespace RocketScienceMultiplaySDK