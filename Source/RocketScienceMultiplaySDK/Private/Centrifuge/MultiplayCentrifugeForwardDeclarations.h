/*
 * #####################################################################################
 *  Rocket Science Multiplay Plugin for Unreal Engine Copyright © 2026 Rocket Science Group
 * #####################################################################################
 */

#pragma once

namespace RocketScienceMultiplaySDK
{
	enum class EDisconnectCode;
	enum class EMethodType;
	enum class EPushType;

	class FCentrifugeClient;

	class FError;
	class FCommand;
	class ICommand;

	class FClientInfo;
	class FStreamPosition;

	// Push messages
	class FConnect;
	class FDisconnect;
	class FJoin;
	class FLeave;
	class FMessage;
	class FPublication;
	class FRefresh;
	class FSubscribe;
	class FUnsubscribe;

	// Command messages
	class FConnectRequest;
	class FHistoryRequest;
	class FPingRequest;
	class FPresenceRequest;
	class FPresenceStatsRequest;
	class FPublishRequest;
	class FRefreshRequest;
	class FRpcRequest;
	class FSendRequest;
	class FSubRefreshRequest;
	class FSubscribeRequest;
	class FUnsubscribeRequest;

	// Reply messages
	class FConnectResult;
	class FHistoryResult;
	class FPingResult;
	class FPresenceResult;
	class FPresenceStatsResult;
	class FPublishResult;
	class FRefreshResult;
	class FRpcResult;
	class FSubRefreshResult;
	class FSubscribeResult;
	class FUnsubscribeResult;
} // namespace RocketScienceMultiplaySDK 