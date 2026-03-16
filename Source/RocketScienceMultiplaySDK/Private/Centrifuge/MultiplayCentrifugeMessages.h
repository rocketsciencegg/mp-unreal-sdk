/*
 * #####################################################################################
 *  Rocket Science Multiplay Plugin for Unreal Engine Copyright © 2026 Rocket Science Group
 * #####################################################################################
 */

#pragma once

#include "Utils/MultiplayJsonHelpers.h"

namespace RocketScienceMultiplaySDK
{
	// enum MethodType {
	//   CONNECT = 0;
	//   SUBSCRIBE = 1;
	//   UNSUBSCRIBE = 2;
	//   PUBLISH = 3;
	//   PRESENCE = 4;
	//   PRESENCE_STATS = 5;
	//   HISTORY = 6;
	//   PING = 7;
	//   SEND = 8;
	//   RPC = 9;
	//   REFRESH = 10;
	//   SUB_REFRESH = 11;
	// }
	enum class EMethodType : int32
	{
		Connect = 0,
		Subscribe = 1,
		Unsubscribe = 2,
		Publish = 3,
		Presence = 4,
		PresenceStats = 5,
		History = 6,
		Ping = 7,
		Send = 8,
		RPC = 9,
		Refresh = 10,
		SubRefresh = 11,
	};

	// enum PushType {
	//   PUBLICATION = 0;
	//   JOIN = 1;
	//   LEAVE = 2;
	//   UNSUBSCRIBE = 3;
	//   MESSAGE = 4;
	//   SUBSCRIBE = 5;
	//   CONNECT = 6;
	//   DISCONNECT = 7;
	//   REFRESH = 8;
	// }
	enum class EPushType : int32
	{
		Publication = 0,
		Join = 1,
		Leave = 2,
		Unsubscribe = 3,
		Message = 4,
		Subscribe = 5,
		Connect = 6,
		Disconnect = 7,
		Refresh = 8,
	};

	enum class EDisconnectCode : int32
	{
		// DisconnectNormal is clean disconnect when client cleanly closed connection.
		DisconnectNormal = 3000,

		// DisconnectShutdown sent when node is going to shut down.
		DisconnectShutdown = 3001,

		// DisconnectInvalidToken sent when client came with invalid token.
		DisconnectInvalidToken = 3002,

		// DisconnectBadRequest sent when client uses malformed protocol frames or wrong order of commands.
		DisconnectBadRequest = 3003,

		// DisconnectServerError sent when internal error occurred on server.
		DisconnectServerError = 3004,

		// DisconnectExpired sent when client connection expired.
		DisconnectExpired = 3005,

		// DisconnectSubExpired sent when client subscription expired.
		DisconnectSubExpired = 3006,

		// DisconnectStale sent to close connection that did not become authenticated in configured interval after dialing.
		DisconnectStale = 3007,

		// DisconnectSlow sent when client can't read messages fast enough.
		DisconnectSlow = 3008,

		// DisconnectWriteError sent when an error occurred while writing to client connection.
		DisconnectWriteError = 3009,

		// DisconnectInsufficientState sent when server detects wrong client position in channel Publication stream. Disconnect allows client to restore missed publications on reconnect.
		DisconnectInsufficientState = 3010,

		// DisconnectForceReconnect sent when server disconnects connection.
		DisconnectForceReconnect = 3011,

		// DisconnectForceNoReconnect sent when server disconnects connection and asks it to not reconnect again.
		DisconnectForceNoReconnect = 3012,

		// DisconnectConnectionLimit can be sent when client connection exceeds a configured connection limit (per user ID or due to other rule).
		DisconnectConnectionLimit = 3013,

		// DisconnectChannelLimit can be sent when client connection exceeds a configured channel limit.
		DisconnectChannelLimit = 3013,
	};

	// message Error {
	//   uint32 code = 1;
	//   string message = 2;
	// }
	class FError : public IJsonReadable
	{
	public:
		virtual ~FError() {}

		virtual bool FromJson(const TSharedPtr<FJsonValue>& JsonValue) override
		{
			const TSharedPtr<FJsonObject>* Object;
			if (!JsonValue->TryGetObject(Object))
				return false;

			bool bParseSuccess = true;

			bParseSuccess &= TryGetJsonValue(*Object, TEXT("code"), Code);
			bParseSuccess &= TryGetJsonValue(*Object, TEXT("message"), Message);

			return bParseSuccess;
		}

	public:
		uint32 Code;
		FString Message;
	};

	class ICommand : public IJsonWritable
	{
	public:
		virtual ~ICommand() {}

		virtual EMethodType GetMethod() const = 0;
	};

	// message Command {
	//   uint32 id = 1;
	//   MethodType method = 2;
	//   bytes params = 3;
	// }
	class FCommand : public IJsonWritable
	{
	public:
		FCommand(uint32 Id, TUniquePtr<ICommand> Command) : Id(Id), Command(MoveTemp(Command)) { }
		virtual ~FCommand() {}

		virtual void WriteJson(JsonWriter& Writer) const override
		{
			Writer->WriteObjectStart();
			Writer->WriteIdentifierPrefix(TEXT("id")); WriteJsonValue(Writer, Id);
			Writer->WriteIdentifierPrefix(TEXT("method")); WriteJsonValue(Writer, static_cast<int32>(Command->GetMethod()));
			Writer->WriteIdentifierPrefix(TEXT("params")); Command->WriteJson(Writer);
			Writer->WriteObjectEnd();
		}

		int32 GetId() const { return Id; }
		EMethodType GetMethod() const { return Command->GetMethod(); }

	private:
		int32 Id;
		TUniquePtr<ICommand> Command;
	};

	// message SubscribeRequest{
	//   string channel = 1;
	//   string token = 2;
	//   bool recover = 3;
	//   // 4-5 skipped here for backwards compatibility.
	//   string epoch = 6;
	//   uint64 offset = 7;
	// }
	class FSubscribeRequest : public ICommand
	{
	public:
		virtual ~FSubscribeRequest() {}

		virtual void WriteJson(JsonWriter& Writer) const override
		{
			Writer->WriteObjectStart();
			if (Channel.IsSet())
			{
				Writer->WriteIdentifierPrefix(TEXT("channel")); WriteJsonValue(Writer, Channel.GetValue());
			}
			if (Token.IsSet())
			{
				Writer->WriteIdentifierPrefix(TEXT("token")); WriteJsonValue(Writer, Token.GetValue());
			}
			if (bRecover.IsSet())
			{
				Writer->WriteIdentifierPrefix(TEXT("recover")); WriteJsonValue<bool>(Writer, bRecover.GetValue());
			}
			if (Epoch.IsSet())
			{
				Writer->WriteIdentifierPrefix(TEXT("epoch")); WriteJsonValue(Writer, Epoch.GetValue());
			}
			if (Offset.IsSet())
			{
				Writer->WriteIdentifierPrefix(TEXT("offset")); WriteJsonValue<int64>(Writer, Offset.GetValue());
			}
			Writer->WriteObjectEnd();
		}

		virtual EMethodType GetMethod() const override
		{
			return EMethodType::Subscribe;
		}

	public:
		TOptional<FString> Channel;
		TOptional<FString> Token;
		TOptional<bool> bRecover;
		TOptional<FString> Epoch;
		TOptional<uint64> Offset; // NOTE: This value may be truncated, it is cast to an int64 because TJsonWriter::WriteValueOnly() does not have overloads for uint32/uint64.
	};

	// message ConnectRequest{
	//   string token = 1;
	//   bytes data = 2;
	//   map<string, SubscribeRequest> subs = 3;
	//   string name = 4;
	//   string version = 5;
	// }
	class FConnectRequest : public ICommand
	{
	public:
		virtual ~FConnectRequest() {}

		virtual void WriteJson(JsonWriter& Writer) const override
		{
			Writer->WriteObjectStart();
			if (Token.IsSet())
			{
				Writer->WriteIdentifierPrefix(TEXT("token")); WriteJsonValue(Writer, Token.GetValue());
			}
			if (Name.IsSet())
			{
				Writer->WriteIdentifierPrefix(TEXT("name")); WriteJsonValue(Writer, Name.GetValue());
			}
			// TODO
			//   bytes data = 2;
			//   map<string, SubscribeRequest> subs = 3;
			if (Version.IsSet())
			{
				Writer->WriteIdentifierPrefix(TEXT("version")); WriteJsonValue(Writer, Version.GetValue());
			}
			Writer->WriteObjectEnd();
		}

		virtual EMethodType GetMethod() const override
		{
			return EMethodType::Connect;
		}

	public:
		TOptional<FString> Token;
		TOptional<FString> Name;
		// TODO
		//   bytes data = 2;
		//   map<string, SubscribeRequest> subs = 3;
		TOptional<FString> Version;
	};

	// message ConnectResult {
	//   string client = 1;
	//   string version = 2;
	//   bool expires = 3;
	//   uint32 ttl = 4;
	//   bytes data = 5;
	//   map<string, SubscribeResult> subs = 6;
	// }
	class FConnectResult : public IJsonReadable
	{
	public:
		virtual ~FConnectResult() {}

		virtual bool FromJson(const TSharedPtr<FJsonValue>& JsonValue) override
		{
			const TSharedPtr<FJsonObject>* Object;
			if (!JsonValue->TryGetObject(Object))
				return false;

			bool bParseSuccess = true;

			bParseSuccess &= TryGetJsonValue(*Object, TEXT("client"), Client);
			bParseSuccess &= TryGetJsonValue(*Object, TEXT("version"), Version);
			bParseSuccess &= TryGetJsonValue(*Object, TEXT("expires"), bExpires);
			bParseSuccess &= TryGetJsonValue(*Object, TEXT("ttl"), TTL);
			// TODO
			//   bytes data = 5;
			//   map<string, SubscribeResult> subs = 6;

			return bParseSuccess;
		}

	public:
		TOptional<FString> Client;
		TOptional<FString> Version;
		TOptional<bool> bExpires;
		TOptional<uint32> TTL;
		// TODO
		//   bytes data = 5;
		//   map<string, SubscribeResult> subs = 6;
	};

	// message ClientInfo {
	//   string user = 1;
	//   string client = 2;
	//   bytes conn_info = 3;
	//   bytes chan_info = 4;
	// }
	class FClientInfo : public IJsonReadable
	{
	public:
		virtual ~FClientInfo() {}

		virtual bool FromJson(const TSharedPtr<FJsonValue>& JsonValue) override
		{
			const TSharedPtr<FJsonObject>* Object;
			if (!JsonValue->TryGetObject(Object))
				return false;

			bool bParseSuccess = true;

			bParseSuccess &= TryGetJsonValue(*Object, TEXT("user"), User);
			bParseSuccess &= TryGetJsonValue(*Object, TEXT("client"), Client);
			// TODO
			//   bytes conn_info = 3;
			//   bytes chan_info = 4;

			return bParseSuccess;
		}

	public:
		TOptional<FString> User;
		TOptional<FString> Client;
		// TODO:
		//   bytes conn_info = 3;
		//   bytes chan_info = 4;
	};

	// message Publication {
	//   // 1-3 skipped here for backwards compatibility.
	//   bytes data = 4;
	//   ClientInfo info = 5;
	//   uint64 offset = 6;
	// }
	class FPublication : public IJsonReadable
	{
	public:
		virtual ~FPublication() {}

		virtual bool FromJson(const TSharedPtr<FJsonValue>& JsonValue) override
		{
			const TSharedPtr<FJsonObject>* Object;
			if (!JsonValue->TryGetObject(Object))
				return false;

			bool bParseSuccess = true;
			
			bParseSuccess &= TryGetJsonValue(*Object, TEXT("data"), Data);

			TSharedPtr<FJsonValue> InfoObject;
			if (TryGetJsonValue(*Object, TEXT("info"), InfoObject))
			{
				FClientInfo InfoFromJson;
				bParseSuccess &= InfoFromJson.FromJson(InfoObject);
				if (bParseSuccess)
				{
					Info = InfoFromJson;
				}
			}

			bParseSuccess &= TryGetJsonValue(*Object, TEXT("offset"), Offset);

			return bParseSuccess;
		}

	public:
		TSharedPtr<FJsonValue> Data;
		TOptional<FClientInfo> Info;
		uint64 Offset;
	};

	// message Join {
	//   ClientInfo info = 1;
	// }
	class FJoin : public IJsonReadable
	{
	public:
		virtual ~FJoin() {}

		virtual bool FromJson(const TSharedPtr<FJsonValue>& JsonValue) override
		{
			const TSharedPtr<FJsonObject>* Object;
			if (!JsonValue->TryGetObject(Object))
				return false;

			bool bParseSuccess = true;

			TSharedPtr<FJsonValue> InfoObject;
			bParseSuccess &= TryGetJsonValue(*Object, TEXT("info"), InfoObject);
			if (bParseSuccess)
			{
				FClientInfo InfoFromJson;
				bParseSuccess &= InfoFromJson.FromJson(InfoObject);
				if (bParseSuccess)
				{
					Info = InfoFromJson;
				}
			}

			return bParseSuccess;
		}

	public:
		TOptional<FClientInfo> Info;
	};

	// message Leave {
	//   ClientInfo info = 1;
	// }
	class FLeave : public IJsonReadable
	{
	public:
		virtual ~FLeave() {}

		virtual bool FromJson(const TSharedPtr<FJsonValue>& JsonValue) override
		{
			const TSharedPtr<FJsonObject>* Object;
			if (!JsonValue->TryGetObject(Object))
				return false;

			bool bParseSuccess = true;

			TSharedPtr<FJsonValue> InfoObject;
			bParseSuccess &= TryGetJsonValue(*Object, TEXT("info"), InfoObject);
			if (bParseSuccess)
			{
				FClientInfo InfoFromJson;
				bParseSuccess &= InfoFromJson.FromJson(InfoObject);
				if (bParseSuccess)
				{
					Info = InfoFromJson;
				}
			}

			return bParseSuccess;
		}

	public:
		TOptional<FClientInfo> Info;
	};

	// message Message {
	//   bytes data = 1;
	// }
	class FMessage : public IJsonReadable
	{
	public:
		virtual ~FMessage() {}

		virtual bool FromJson(const TSharedPtr<FJsonValue>& JsonValue) override
		{
			const TSharedPtr<FJsonObject>* Object;
			if (!JsonValue->TryGetObject(Object))
				return false;

			TSharedPtr<FJsonValue> DataObject;
			bool bParseSuccess = true;
			bParseSuccess &= TryGetJsonValue(*Object, TEXT("data"), DataObject);
			if (bParseSuccess)
			{
				Data = DataObject;
			}

			return bParseSuccess;
		}

	public:
		TOptional<TSharedPtr<FJsonValue>> Data;
	};

	// message Unsubscribe {
	//   // Field 1 removed (bool resubscribe).
	// }
	class FUnsubscribe : public IJsonReadable
	{
	public:
		virtual ~FUnsubscribe() {}

		virtual bool FromJson(const TSharedPtr<FJsonValue>& JsonValue) override
		{
			return true;
		}
	};

	// message Subscribe {
	//   bool recoverable = 1;
	//   // 2-3 skipped here for backwards compatibility.
	//   string epoch = 4;
	//   uint64 offset = 5;
	//   bool positioned = 6;
	//   bytes data = 7;
	// }
	class FSubscribe : public IJsonReadable
	{
	public:
		virtual ~FSubscribe() {}

		virtual bool FromJson(const TSharedPtr<FJsonValue>& JsonValue) override
		{
			const TSharedPtr<FJsonObject>* Object;
			if (!JsonValue->TryGetObject(Object))
				return false;

			bool bParseSuccess = true;

			// TODO:
			//   bool recoverable = 1;
			//   string epoch = 4;
			//   uint64 offset = 5;
			//   bool positioned = 6;
			//   bytes data = 7;

			return bParseSuccess;
		}

	public:
		// TODO:
		//   bool recoverable = 1;
		//   string epoch = 4;
		//   uint64 offset = 5;
		//   bool positioned = 6;
		//   bytes data = 7;
	};

	// message Connect {
	//   string client = 1;
	//   string version = 2;
	//   bytes data = 3;
	//   map<string, SubscribeResult> subs = 4;
	//   bool expires = 5;
	//   uint32 ttl = 6;
	// }
	class FConnect : public IJsonReadable
	{
	public:
		virtual ~FConnect() {}

		virtual bool FromJson(const TSharedPtr<FJsonValue>& JsonValue) override
		{
			const TSharedPtr<FJsonObject>* Object;
			if (!JsonValue->TryGetObject(Object))
				return false;

			bool bParseSuccess = true;

			bParseSuccess &= TryGetJsonValue(*Object, TEXT("client"), Client);
			bParseSuccess &= TryGetJsonValue(*Object, TEXT("version"), Version);
			// TODO:
			//   bytes data = 3;
			//   map<string, SubscribeResult> subs = 4;
			bParseSuccess &= TryGetJsonValue(*Object, TEXT("expires"), bExpires);
			bParseSuccess &= TryGetJsonValue(*Object, TEXT("ttl"), TTL);

			return bParseSuccess;
		}

	public:
		FString Client;
		FString Version;
		// TODO:
		//   bytes data = 3;
		//   map<string, SubscribeResult> subs = 4;
		bool bExpires;
		uint32 TTL;
	};

	// message SubscribeResult {
	//   bool expires = 1;
	//   uint32 ttl = 2;
	//   bool recoverable = 3;
	//   // 4-5 skipped here for backwards compatibility.
	//   string epoch = 6;
	//   repeated Publication publications = 7;
	//   bool recovered = 8;
	//   uint64 offset = 9;
	//   bool positioned = 10;
	//   bytes data = 11;
	// }
	class FSubscribeResult : public IJsonReadable
	{
	public:
		virtual ~FSubscribeResult() {}

		virtual bool FromJson(const TSharedPtr<FJsonValue>& JsonValue) override
		{
			const TSharedPtr<FJsonObject>* Object;
			if (!JsonValue->TryGetObject(Object))
				return false;

			bool bParseSuccess = true;

			bParseSuccess &= TryGetJsonValue(*Object, TEXT("expires"), bExpires);
			bParseSuccess &= TryGetJsonValue(*Object, TEXT("ttl"), TTL);
			bParseSuccess &= TryGetJsonValue(*Object, TEXT("recoverable"), bRecoverable);
			bParseSuccess &= TryGetJsonValue(*Object, TEXT("epoch"), Epoch);
			// TODO:
			//   repeated Publication publications = 7;
			bParseSuccess &= TryGetJsonValue(*Object, TEXT("recovered"), bRecovered);
			bParseSuccess &= TryGetJsonValue(*Object, TEXT("offset"), Offset);
			bParseSuccess &= TryGetJsonValue(*Object, TEXT("positioned"), bPositioned);
			// TODO:
			//   bytes data = 11;

			return bParseSuccess;
		}

	public:
		TOptional<bool> bExpires;
		TOptional<uint32> TTL;
		TOptional<bool> bRecoverable;
		TOptional<FString> Epoch;
		// TODO:
		//   repeated Publication publications = 7;
		TOptional<bool> bRecovered;
		TOptional<uint64> Offset;
		TOptional<bool> bPositioned;
		// TODO:
		//   bytes data = 11;
	};

	// message Disconnect {
	//   uint32 code = 1;
	//   string reason = 2;
	//   bool reconnect = 3;
	// }
	class FDisconnect : public IJsonReadable
	{
	public:
		virtual ~FDisconnect() {}

		virtual bool FromJson(const TSharedPtr<FJsonValue>& JsonValue) override
		{
			const TSharedPtr<FJsonObject>* Object;
			if (!JsonValue->TryGetObject(Object))
				return false;

			bool bParseSuccess = true;

			bParseSuccess &= TryGetJsonValue(*Object, TEXT("code"), Code);
			bParseSuccess &= TryGetJsonValue(*Object, TEXT("reason"), Reason);
			bParseSuccess &= TryGetJsonValue(*Object, TEXT("reconnect"), bReconnect);

			return bParseSuccess;
		}

	public:
		uint32 Code;
		FString Reason;
		bool bReconnect;
	};

	// message Refresh {
	//   bool expires = 1;
	//   uint32 ttl = 2;
	// }
	class FRefresh : public IJsonReadable
	{
	public:
		virtual ~FRefresh() {}

		virtual bool FromJson(const TSharedPtr<FJsonValue>& JsonValue) override
		{
			const TSharedPtr<FJsonObject>* Object;
			if (!JsonValue->TryGetObject(Object))
				return false;

			bool bParseSuccess = true;

			// TODO:
			//   bool expires = 1;
			//   uint32 ttl = 2;

			return bParseSuccess;
		}

	public:
		// TODO:
		//   bool expires = 1;
		//   uint32 ttl = 2;
	};

	// message RefreshResult {
	//   string client = 1;
	//   string version = 2;
	//   bool expires = 3;
	//   uint32 ttl = 4;
	// }
	class FRefreshResult : public IJsonReadable
	{
	public:
		virtual ~FRefreshResult() {}

		virtual bool FromJson(const TSharedPtr<FJsonValue>& JsonValue) override
		{
			const TSharedPtr<FJsonObject>* Object;
			if (!JsonValue->TryGetObject(Object))
				return false;

			bool bParseSuccess = true;

			bParseSuccess &= TryGetJsonValue(*Object, TEXT("client"), Client);
			bParseSuccess &= TryGetJsonValue(*Object, TEXT("version"), Version);
			bParseSuccess &= TryGetJsonValue(*Object, TEXT("expires"), bExpires);
			bParseSuccess &= TryGetJsonValue(*Object, TEXT("ttl"), TTL);

			return bParseSuccess;
		}

	public:
		FString Client;
		FString Version;
		bool bExpires;
		uint32 TTL;
	};

	// message PresenceRequest {
	//   string channel = 1;
	// }
	class FPresenceRequest : public ICommand
	{
	public:
		virtual ~FPresenceRequest() {}

		virtual void WriteJson(JsonWriter& Writer) const override
		{
			Writer->WriteObjectStart();
			Writer->WriteIdentifierPrefix(TEXT("channel")); WriteJsonValue(Writer, Channel);
			Writer->WriteObjectEnd();
		}

		virtual EMethodType GetMethod() const override
		{
			return EMethodType::Presence;
		}

	public:
		FString Channel;
	};

	// message PresenceResult {
	//   map<string, ClientInfo> presence = 1;
	// }
	class FPresenceResult : public IJsonReadable
	{
	public:
		virtual ~FPresenceResult() {}

		virtual bool FromJson(const TSharedPtr<FJsonValue>& JsonValue) override
		{
			const TSharedPtr<FJsonObject>* Object;
			if (!JsonValue->TryGetObject(Object))
				return false;

			bool bParseSuccess = true;

			// TODO:
			//   map<string, ClientInfo> presence = 1;

			return bParseSuccess;
		}

	public:
		// TODO:
		//   map<string, ClientInfo> presence = 1;
	};

	// message UnsubscribeRequest {
	//   string channel = 1;
	// }
	class FUnsubscribeRequest : public ICommand
	{
	public:
		virtual ~FUnsubscribeRequest() {}

		virtual void WriteJson(JsonWriter& Writer) const override
		{
			Writer->WriteObjectStart();
			Writer->WriteIdentifierPrefix(TEXT("channel")); WriteJsonValue(Writer, Channel);
			Writer->WriteObjectEnd();
		}

		virtual EMethodType GetMethod() const override
		{
			return EMethodType::Unsubscribe;
		}

	public:
		FString Channel;
	};

	// message UnsubscribeResult {}
	class FUnsubscribeResult : public IJsonReadable
	{
	public:
		virtual ~FUnsubscribeResult() {}

		virtual bool FromJson(const TSharedPtr<FJsonValue>& JsonValue) override
		{
			return true;
		}
	};

	// message PublishRequest {
	//   string channel = 1;
	//   bytes data = 2;
	// }
	class FPublishRequest : public ICommand
	{
	public:
		virtual ~FPublishRequest() {}

		virtual void WriteJson(JsonWriter& Writer) const override
		{
			Writer->WriteObjectStart();
			Writer->WriteIdentifierPrefix(TEXT("channel")); WriteJsonValue(Writer, Channel);
			// TODO:
			//   bytes data = 2;
			Writer->WriteObjectEnd();
		}

		virtual EMethodType GetMethod() const override
		{
			return EMethodType::Publish;
		}

	public:
		FString Channel;
		// TODO:
		//   bytes data = 2;
	};

	// message PublishResult {}
	class FPublishResult : public IJsonReadable
	{
	public:
		virtual ~FPublishResult() {}

		virtual bool FromJson(const TSharedPtr<FJsonValue>& JsonValue) override
		{
			return true;
		}
	};

	// message PresenceStatsRequest {
	//   string channel = 1;
	// }
	class FPresenceStatsRequest : public ICommand
	{
	public:
		virtual ~FPresenceStatsRequest() {}

		virtual void WriteJson(JsonWriter& Writer) const override
		{
			Writer->WriteObjectStart();
			Writer->WriteIdentifierPrefix(TEXT("channel")); WriteJsonValue(Writer, Channel);
			Writer->WriteObjectEnd();
		}

		virtual EMethodType GetMethod() const override
		{
			return EMethodType::PresenceStats;
		}

	public:
		FString Channel;
	};

	// message PresenceStatsResult {
	//   uint32 num_clients = 1;
	//   uint32 num_users = 2;
	// }
	class FPresenceStatsResult : public IJsonReadable
	{
	public:
		virtual ~FPresenceStatsResult() {}

		virtual bool FromJson(const TSharedPtr<FJsonValue>& JsonValue) override
		{
			const TSharedPtr<FJsonObject>* Object;
			if (!JsonValue->TryGetObject(Object))
				return false;

			bool bParseSuccess = true;

			bParseSuccess &= TryGetJsonValue(*Object, TEXT("num_clients"), NumClients);
			bParseSuccess &= TryGetJsonValue(*Object, TEXT("num_users"), NumUsers);

			return bParseSuccess;
		}

	public:
		uint32 NumClients;
		uint32 NumUsers;
	};

	// message StreamPosition {
	//   uint64 offset = 1;
	//   string epoch = 2;
	// }
	class FStreamPosition : public IJsonWritable
	{
	public:
		virtual ~FStreamPosition() {}

		virtual void WriteJson(JsonWriter& Writer) const override
		{
			Writer->WriteObjectStart();
			Writer->WriteIdentifierPrefix(TEXT("offset")); WriteJsonValue(Writer, static_cast<int64>(Offset));
			Writer->WriteIdentifierPrefix(TEXT("epoch")); WriteJsonValue(Writer, Epoch);
			Writer->WriteObjectEnd();
		}

	public:
		uint64 Offset;
		FString Epoch;
	};

	// message HistoryRequest {
	//   string channel = 1;
	//   // 2-6 skipped here for backwards compatibility.
	//   int32 limit = 7;
	//   StreamPosition since = 8;
	//   bool reverse = 9;
	// }
	class FHistoryRequest : public ICommand
	{
	public:
		virtual ~FHistoryRequest() {}

		virtual void WriteJson(JsonWriter& Writer) const override
		{
			Writer->WriteObjectStart();
			Writer->WriteIdentifierPrefix(TEXT("channel")); WriteJsonValue(Writer, Channel);
			Writer->WriteIdentifierPrefix(TEXT("limit")); WriteJsonValue(Writer, Limit);
			Writer->WriteIdentifierPrefix(TEXT("since")); Since.WriteJson(Writer);
			Writer->WriteIdentifierPrefix(TEXT("reverse")); WriteJsonValue(Writer, bReverse);
			Writer->WriteObjectEnd();
		}

		virtual EMethodType GetMethod() const override
		{
			return EMethodType::History;
		}

	public:
		FString Channel;
		int32 Limit;
		FStreamPosition Since;
		bool bReverse;
	};

	// message HistoryResult {
	//   repeated Publication publications = 1;
	//   string epoch = 2;
	//   uint64 offset = 3;
	// }
	class FHistoryResult : public IJsonReadable
	{
	public:
		virtual ~FHistoryResult() {}

		virtual bool FromJson(const TSharedPtr<FJsonValue>& JsonValue) override
		{
			const TSharedPtr<FJsonObject>* Object;
			if (!JsonValue->TryGetObject(Object))
				return false;

			bool bParseSuccess = true;

			// TODO:
			//   repeated Publication publications = 1;
			bParseSuccess &= TryGetJsonValue(*Object, TEXT("epoch"), Epoch);
			bParseSuccess &= TryGetJsonValue(*Object, TEXT("offset"), Offset);

			return bParseSuccess;
		}

	public:
		// TODO:
		//   repeated Publication publications = 1;
		FString Epoch;
		uint64 Offset;
	};

	// message SendRequest{
	//   bytes data = 1;
	// }
	class FSendRequest : public ICommand
	{
	public:
		virtual ~FSendRequest() {}

		virtual void WriteJson(JsonWriter& Writer) const override
		{
			Writer->WriteObjectStart();
			// TODO:
			//   bytes data = 1;
			Writer->WriteObjectEnd();
		}

		virtual EMethodType GetMethod() const override
		{
			return EMethodType::Send;
		}

	public:
		// TODO:
		//   bytes data = 1;
	};

	// message RPCRequest{
	//   bytes data = 1;
	//   string method = 2;
	// }
	class FRpcRequest : public ICommand
	{
	public:
		virtual ~FRpcRequest() {}

		virtual void WriteJson(JsonWriter& Writer) const override
		{
			Writer->WriteObjectStart();
			// TODO:
			//   bytes data = 1;
			Writer->WriteIdentifierPrefix(TEXT("method")); WriteJsonValue(Writer, Method);
			Writer->WriteObjectEnd();
		}

		virtual EMethodType GetMethod() const override
		{
			return EMethodType::RPC;
		}

	public:
		// TODO:
		//   bytes data = 1;
		FString Method;
	};

	// message RPCResult {
	//   bytes data = 1;
	// }
	class FRpcResult : public IJsonReadable
	{
	public:
		virtual ~FRpcResult() {}

		virtual bool FromJson(const TSharedPtr<FJsonValue>& JsonValue) override
		{
			// TODO:
			//   bytes data = 1;
			return false;
		}

	public:
		// TODO:
		//   bytes data = 1;
	};

	// message RefreshRequest {
	//   string token = 1;
	// }
	class FRefreshRequest : public ICommand
	{
	public:
		virtual ~FRefreshRequest() {}

		virtual void WriteJson(JsonWriter& Writer) const override
		{
			Writer->WriteObjectStart();
			Writer->WriteIdentifierPrefix(TEXT("token")); WriteJsonValue(Writer, Token);
			Writer->WriteObjectEnd();
		}

		virtual EMethodType GetMethod() const override
		{
			return EMethodType::Refresh;
		}

	public:
		FString Token;
	};

	// message SubRefreshRequest {
	//   string channel = 1;
	//   string token = 2;
	// }
	class FSubRefreshRequest : public ICommand
	{
	public:
		virtual ~FSubRefreshRequest() {}

		virtual void WriteJson(JsonWriter& Writer) const override
		{
			Writer->WriteObjectStart();
			Writer->WriteIdentifierPrefix(TEXT("channel")); WriteJsonValue(Writer, Channel);
			Writer->WriteIdentifierPrefix(TEXT("token")); WriteJsonValue(Writer, Token);
			Writer->WriteObjectEnd();
		}

		virtual EMethodType GetMethod() const override
		{
			return EMethodType::SubRefresh;
		}

	public:
		FString Channel;
		FString Token;
	};

	// message SubRefreshResult {
	//   bool expires = 1;
	//   uint32 ttl = 2;
	// }
	class FSubRefreshResult : public IJsonReadable
	{
	public:
		virtual ~FSubRefreshResult() {}

		virtual bool FromJson(const TSharedPtr<FJsonValue>& JsonValue) override
		{
			const TSharedPtr<FJsonObject>* Object;
			if (!JsonValue->TryGetObject(Object))
				return false;

			bool bParseSuccess = true;

			bParseSuccess &= TryGetJsonValue(*Object, TEXT("expires"), bExpires);
			bParseSuccess &= TryGetJsonValue(*Object, TEXT("ttl"), TTL);

			return bParseSuccess;
		}

	public:
		bool bExpires;
		uint32 TTL;
	};

	// message PingRequest {}
	class FPingRequest : public ICommand
	{
	public:
		virtual ~FPingRequest() {}

		virtual void WriteJson(JsonWriter& Writer) const override
		{
			Writer->WriteObjectStart();
			Writer->WriteObjectEnd();
		}

		virtual EMethodType GetMethod() const override
		{
			return EMethodType::Ping;
		}
	};

	// message PingResult {}
	class FPingResult : public IJsonReadable
	{
	public:
		virtual ~FPingResult() {}

		virtual bool FromJson(const TSharedPtr<FJsonValue>& JsonValue) override
		{
			return true;
		}
	};
} // namespace RocketScienceMultiplaySDK
