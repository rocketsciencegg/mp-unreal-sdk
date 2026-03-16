/*
 * #####################################################################################
 *  Rocket Science Multiplay Plugin for Unreal Engine Copyright © 2026 Rocket Science Group
 * #####################################################################################
 */

#include "MultiplayCentrifugeClient.h"
#include "MultiplayCentrifugeLog.h"
#include "MultiplayCentrifugeMessages.h"
#include "WebSocketsModule.h"
#include "IWebSocket.h"
#include "Runtime/Launch/Resources/Version.h"

namespace RocketScienceMultiplaySDK
{
	FCentrifugeClient::FCentrifugeClient(FString Url) : Url(Url), Id(kInitialMsgId), Status(EConnectionStatus::Disconnected)
	{
		WebSocket = FWebSocketsModule::Get().CreateWebSocket(Url, TEXT("ws"));

		WebSocket->OnConnected().AddRaw(this, &FCentrifugeClient::OnConnected);
		WebSocket->OnConnectionError().AddRaw(this, &FCentrifugeClient::OnConnectionError);
		WebSocket->OnClosed().AddRaw(this, &FCentrifugeClient::OnClosed);
		WebSocket->OnMessage().AddRaw(this, &FCentrifugeClient::OnMessage);
#if (ENGINE_MAJOR_VERSION == 4 && ENGINE_MINOR_VERSION >= 25) || (ENGINE_MAJOR_VERSION > 4)
		// This callback was added in UE 4.25.
		// This callback is used for logging purposes so its absence is acceptable.
		WebSocket->OnMessageSent().AddRaw(this, &FCentrifugeClient::OnMessageSent);
#endif
	}

	FCentrifugeClient::~FCentrifugeClient()
	{
		WebSocket->OnConnected().RemoveAll(this);
		WebSocket->OnConnectionError().RemoveAll(this);
		WebSocket->OnClosed().RemoveAll(this);
		WebSocket->OnMessage().RemoveAll(this);
#if (ENGINE_MAJOR_VERSION == 4 && ENGINE_MINOR_VERSION >= 25) || (ENGINE_MAJOR_VERSION > 4)
		// This callback was added in UE 4.25.
		// This callback is used for logging purposes so its absence is acceptable.
		WebSocket->OnMessageSent().RemoveAll(this);
#endif
	}

	void FCentrifugeClient::Connect(const FConnectRequest& Request)
	{
		if (Status == EConnectionStatus::Connected)
		{
			UE_LOG(LogRSCentrifuge, Warning, TEXT("Attempted to initiate a connection when the Centrifuge client is connected."));
		}
		else if (Status == EConnectionStatus::Connecting)
		{
			UE_LOG(LogRSCentrifuge, Warning, TEXT("Attempted to initiate a connection when the Centrifuge client is attempting to connect."));
		}
		else if (Status == EConnectionStatus::Disconnecting)
		{
			UE_LOG(LogRSCentrifuge, Warning, TEXT("Attempted to initiate a connection when the Centrifuge client is attempting to disconnect."));
		}
		else
		{
			ChangeConnectionStatus(EConnectionStatus::Connecting);

			WebSocket->Connect();
		}
	}

	void FCentrifugeClient::Subscribe(const FSubscribeRequest& Request)
	{
		SendRequest<FSubscribeRequest>(Request);
	}

	void FCentrifugeClient::Unsubscribe(const FUnsubscribeRequest& Request)
	{
		SendRequest<FUnsubscribeRequest>(Request);
	}

	void FCentrifugeClient::Publish(const FPublishRequest& Request)
	{
		SendRequest<FPublishRequest>(Request);
	}

	void FCentrifugeClient::Presence(const FPresenceRequest& Request)
	{
		SendRequest<FPresenceRequest>(Request);
	}

	void FCentrifugeClient::PresenceStats(const FPresenceStatsRequest& Request)
	{
		SendRequest<FPresenceStatsRequest>(Request);
	}

	void FCentrifugeClient::History(const FHistoryRequest& Request)
	{
		SendRequest<FHistoryRequest>(Request);
	}

	void FCentrifugeClient::Ping(const FPingRequest& Request)
	{
		SendRequest<FPingRequest>(Request);
	}

	void FCentrifugeClient::Send(const FSendRequest& Request)
	{
		SendRequest<FSendRequest>(Request);
	}

	void FCentrifugeClient::Rpc(const FRpcRequest& Request)
	{
		SendRequest<FRpcRequest>(Request);
	}

	void FCentrifugeClient::Refresh(const FRefreshRequest& Request)
	{
		SendRequest<FRefreshRequest>(Request);
	}

	void FCentrifugeClient::SubRefresh(const FSubRefreshRequest& Request)
	{
		SendRequest<FSubRefreshRequest>(Request);
	}

	void FCentrifugeClient::Disconnect()
	{
		if (Status == EConnectionStatus::Disconnecting)
		{
			UE_LOG(LogRSCentrifuge, Warning, TEXT("Attempted to disconnect when the Centrifuge client is attempting to disconnect."));
		}
		else if (Status == EConnectionStatus::Disconnected)
		{
			UE_LOG(LogRSCentrifuge, Warning, TEXT("Attempted to disconnect when the Centrifuge client is disconnected."));
		}
		else if (Status == EConnectionStatus::Connecting)
		{
			UE_LOG(LogRSCentrifuge, Warning, TEXT("Attempted to disconnect when the Centrifuge client is attempting to connect."));
		}
		else
		{
			ChangeConnectionStatus(EConnectionStatus::Disconnecting);

			WebSocket->Close();
		}
	}

	void FCentrifugeClient::OnConnected()
	{
		ChangeConnectionStatus(EConnectionStatus::Connected);

		UE_LOG(LogRSCentrifuge, Log, TEXT("OnConnected()"));

		// TODO: Use argument from FCentrifugeClient::Connect() for FConnectRequest.
		FConnectRequest Request;
		SendRequest<FConnectRequest>(Request);
	}

	void FCentrifugeClient::OnConnectionError(const FString& Error)
	{
#if WITH_EDITOR
		UE_LOG(LogRSCentrifuge, Warning, TEXT("OnConnectionError(%s)"), *Error);
#else
		UE_LOG(LogRSCentrifuge, Error, TEXT("OnConnectionError(%s)"), *Error);
#endif

		// TODO: Implement this method.
	}

	void FCentrifugeClient::OnClosed(int32 StatusCode, const FString& Reason, bool bWasClean)
	{
		ChangeConnectionStatus(EConnectionStatus::Disconnected);

		UE_LOG(LogRSCentrifuge, Log, TEXT("OnClosed(%d, %s, %d)"), StatusCode, *Reason, bWasClean);

		// TODO: Implement this method.
	}

	void FCentrifugeClient::OnMessage(const FString& MessageString)
	{
		UE_LOG(LogRSCentrifuge, Log, TEXT("OnMessage(%s)"), *MessageString);

		ParseCentrifugeMessages(MessageString);
	}

	void FCentrifugeClient::OnMessageSent(const FString& MessageString)
	{
		UE_LOG(LogRSCentrifuge, Log, TEXT("OnMessageSent(%s)"), *MessageString);
	}

	void FCentrifugeClient::ChangeConnectionStatus(EConnectionStatus NewStatus)
	{
		UE_LOG(LogRSCentrifuge, Verbose, TEXT("Changing connection status from %d to %d."), Status, NewStatus);

		Status = NewStatus;
		ConnectionStatusChanged.Broadcast(Status);
	}

	uint32 FCentrifugeClient::GetNextMessageId()
	{
		uint32 NextMessageId = Id;

		Id += 1;

		// 0 is reserved for Push messages, skip it when we wraparound.
		if (Id == kPushId)
		{
			Id = kInitialMsgId;
		}

		return NextMessageId;
	}

	template <typename T>
	void FCentrifugeClient::SendRequest(const T& Request)
	{
		uint32 MessageId = GetNextMessageId();

		TUniquePtr<FCommand> Command = MakeUnique<FCommand>(MessageId, MakeUnique<T>(Request));

		FRequest OutgoingRequest;
		OutgoingRequest.Id = MessageId;
		OutgoingRequest.Method = Request.GetMethod();
		Requests.Add(MessageId, OutgoingRequest);

		FString JsonBody;
		JsonWriter Writer = TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&JsonBody);
		Command->WriteJson(Writer);
		Writer->Close();

		WebSocket->Send(JsonBody);
	}

	bool FCentrifugeClient::TryGetError(const TSharedPtr<FJsonObject>& JsonObject)
	{
		TSharedPtr<FJsonValue> ErrorObject;
		if (!TryGetJsonValue(JsonObject, TEXT("error"), ErrorObject))
		{
			return false;
		}

		FError Error;
		if (Error.FromJson(ErrorObject))
		{
			UE_LOG(LogRSCentrifuge, Error, TEXT("Encountered a Centrifuge error: %d - %s"), Error.Code, *Error.Message);

			// TODO: Respond to the error.

			return true;
		}

		return false;
	}

	bool FCentrifugeClient::TryGetReply(const TSharedPtr<FJsonObject>& JsonObject)
	{
		uint32 ReplyId;
		if (!TryGetJsonValue(JsonObject, TEXT("id"), ReplyId))
		{
			return false;
		}

		FRequest Request;
		if (!Requests.RemoveAndCopyValue(ReplyId, Request))
		{
			UE_LOG(LogRSCentrifuge, Error, TEXT("Failed to locate request with ID %d."), ReplyId);
			return false;
		}

		TSharedPtr<FJsonValue> ResultJsonValue;
		if (!TryGetJsonValue(JsonObject, TEXT("result"), ResultJsonValue))
		{
			return false;
		}

		switch (Request.Method)
		{
		case EMethodType::Connect:
		{
			FConnectResult Reply;
			if (Reply.FromJson(ResultJsonValue))
			{
				ConnectReply.Broadcast(Reply);

				// TODO: Should FCentrifugeClient be storing this information?

				return true;
			}
			else
			{
				UE_LOG(LogRSCentrifuge, Error, TEXT("Failed to parse ConnectResult"));
			}

			break;
		}
		case EMethodType::Subscribe:
		{
			FSubscribeResult Reply;
			if (Reply.FromJson(ResultJsonValue))
			{
				SubscribeReply.Broadcast(Reply);

				return true;
			}
			else
			{
				UE_LOG(LogRSCentrifuge, Error, TEXT("Failed to parse SubscribeResult"));
			}

			break;
		}
		case EMethodType::Unsubscribe:
		{
			FUnsubscribeResult Reply;
			if (Reply.FromJson(ResultJsonValue))
			{
				UnsubscribeReply.Broadcast(Reply);

				return true;
			}
			else
			{
				UE_LOG(LogRSCentrifuge, Error, TEXT("Failed to parse UnsubscribeResult"));
			}

			break;
		}
		case EMethodType::Publish:
		{
			FPublishResult Reply;
			if (Reply.FromJson(ResultJsonValue))
			{
				PublishReply.Broadcast(Reply);

				return true;
			}
			else
			{
				UE_LOG(LogRSCentrifuge, Error, TEXT("Failed to parse PublishResult"));
			}

			break;
		}
		case EMethodType::Presence:
		{
			FPresenceResult Reply;
			if (Reply.FromJson(ResultJsonValue))
			{
				PresenceReply.Broadcast(Reply);

				return true;
			}
			else
			{
				UE_LOG(LogRSCentrifuge, Error, TEXT("Failed to parse PresenceResult"));
			}

			break;
		}
		case EMethodType::PresenceStats:
		{
			FPresenceStatsResult Reply;
			if (Reply.FromJson(ResultJsonValue))
			{
				PresenceStatsReply.Broadcast(Reply);

				return true;
			}
			else
			{
				UE_LOG(LogRSCentrifuge, Error, TEXT("Failed to parse PresenceStatsResult"));
			}

			break;
		}
		case EMethodType::History:
		{
			FHistoryResult Reply;
			if (Reply.FromJson(ResultJsonValue))
			{
				HistoryReply.Broadcast(Reply);

				return true;
			}
			else
			{
				UE_LOG(LogRSCentrifuge, Error, TEXT("Failed to parse HistoryResult"));
			}

			break;
		}
		case EMethodType::Ping:
		{
			FPingResult Reply;
			if (Reply.FromJson(ResultJsonValue))
			{
				PingReply.Broadcast(Reply);

				return true;
			}
			else
			{
				UE_LOG(LogRSCentrifuge, Error, TEXT("Failed to parse PingResult"));
			}

			break;
		}
		case EMethodType::Send:
		{
			// A Send message doesn't have a corresponding Reply.
			return true;
		}
		case EMethodType::RPC:
		{
			FRpcResult Reply;
			if (Reply.FromJson(ResultJsonValue))
			{
				RpcReply.Broadcast(Reply);

				return true;
			}
			else
			{
				UE_LOG(LogRSCentrifuge, Error, TEXT("Failed to parse RpcResult"));
			}

			break;
		}
		case EMethodType::Refresh:
		{
			FRefreshResult Reply;
			if (Reply.FromJson(ResultJsonValue))
			{
				RefreshReply.Broadcast(Reply);

				return true;
			}
			else
			{
				UE_LOG(LogRSCentrifuge, Error, TEXT("Failed to parse RefreshResult"));
			}

			break;
		}
		case EMethodType::SubRefresh:
		{
			FSubRefreshResult Reply;
			if (Reply.FromJson(ResultJsonValue))
			{
				SubRefreshReply.Broadcast(Reply);

				return true;
			}
			else
			{
				UE_LOG(LogRSCentrifuge, Error, TEXT("Failed to parse SubRefreshResult"));
			}

			break;
		}
		default:
		{
			UE_LOG(LogRSCentrifuge, Error, TEXT("Encountered unrecognized Reply method %d"), Request.Method);
		}
		}

		return false;
	}

	bool FCentrifugeClient::TryGetPush(const TSharedPtr<FJsonObject>& JsonObject)
	{
		TSharedPtr<FJsonValue> ResultJsonValue;
		if (!TryGetJsonValue(JsonObject, TEXT("result"), ResultJsonValue))
		{
			return false;
		}

		const TSharedPtr<FJsonObject>* ResultObject;
		if (!ResultJsonValue->TryGetObject(ResultObject))
		{
			return false;
		}

		TSharedPtr<FJsonValue> DataJsonValue;
		if (!TryGetJsonValue(*ResultObject, TEXT("data"), DataJsonValue))
		{
			return false;
		}

		EPushType PushType;
		if (!TryGetJsonValue(*ResultObject, TEXT("type"), (int32&)PushType))
		{
			// If we're unable to retrieve the "type" field, then assume that this message is a PUBLICATION.
			// I've observed that the "type" field is omitted when testing on Multiplay.
			PushType = EPushType::Publication;
		}

		switch (PushType)
		{
		case EPushType::Publication:
		{
			FPublication Push;
			if (Push.FromJson(DataJsonValue))
			{
				PublicationPush.Broadcast(Push);

				return true;
			}
			else
			{
				UE_LOG(LogRSCentrifuge, Error, TEXT("Failed to parse Publication"));
			}

			break;
		}
		case EPushType::Join:
		{
			FJoin Push;
			if (Push.FromJson(DataJsonValue))
			{
				JoinPush.Broadcast(Push);

				return true;
			}
			else
			{
				UE_LOG(LogRSCentrifuge, Error, TEXT("Failed to parse Join"));
			}

			break;
		}
		case EPushType::Leave:
		{
			FLeave Push;
			if (Push.FromJson(DataJsonValue))
			{
				LeavePush.Broadcast(Push);

				return true;
			}
			else
			{
				UE_LOG(LogRSCentrifuge, Error, TEXT("Failed to parse Leave"));
			}

			break;
		}
		case EPushType::Unsubscribe:
		{
			FUnsubscribe Push;
			if (Push.FromJson(DataJsonValue))
			{
				UnsubscribePush.Broadcast(Push);

				return true;
			}
			else
			{
				UE_LOG(LogRSCentrifuge, Error, TEXT("Failed to parse Unsubscribe"));
			}

			break;
		}
		case EPushType::Message:
		{
			FMessage Push;
			if (Push.FromJson(DataJsonValue))
			{
				MessagePush.Broadcast(Push);

				return true;
			}
			else
			{
				UE_LOG(LogRSCentrifuge, Error, TEXT("Failed to parse Message"));
			}

			break;
		}
		case EPushType::Subscribe:
		{
			FSubscribe Push;
			if (Push.FromJson(DataJsonValue))
			{
				SubscribePush.Broadcast(Push);

				return true;
			}
			else
			{
				UE_LOG(LogRSCentrifuge, Error, TEXT("Failed to parse Subscribe"));
			}

			break;
		}
		case EPushType::Connect:
		{
			FConnect Push;
			if (Push.FromJson(DataJsonValue))
			{
				ConnectPush.Broadcast(Push);

				return true;
			}
			else
			{
				UE_LOG(LogRSCentrifuge, Error, TEXT("Failed to parse Connect"));
			}

			break;
		}
		case EPushType::Disconnect:
		{
			FDisconnect Push;
			if (Push.FromJson(DataJsonValue))
			{
				DisconnectPush.Broadcast(Push);

				return true;
			}
			else
			{
				UE_LOG(LogRSCentrifuge, Error, TEXT("Failed to parse Disconnect"));
			}

			break;
		}
		case EPushType::Refresh:
		{
			FRefresh Push;
			if (Push.FromJson(DataJsonValue))
			{
				RefreshPush.Broadcast(Push);

				return true;
			}
			else
			{
				UE_LOG(LogRSCentrifuge, Error, TEXT("Failed to parse Refresh"));
			}

			break;
		}
		default:
		{
			UE_LOG(LogRSCentrifuge, Error, TEXT("Encountered an unrecognized Push type %d"), PushType);

			break;
		}
		}

		return false;
	}

	void FCentrifugeClient::ParseCentrifugeMessages(const FString& MessageString)
	{
		// Centrifuge may transmit multiple messages at a time using an LF character as a delimiter.
		int32 IndexOfDelimiter;
		bool bContainsMultipleMessages = MessageString.FindChar(TCHAR{'\n'}, IndexOfDelimiter);
		if (bContainsMultipleMessages)
		{
			const TCHAR* Delimiter = TEXT("\n");

			TArray<FString> MessageArray;
			int32 Count = MessageString.ParseIntoArray(MessageArray, Delimiter, true);

			for (int32 i = 0; i < Count; i++)
			{
				const FString& Message = MessageArray[i];

				UE_LOG(LogRSCentrifuge, Log, TEXT("Attempting to parse message %d of %d: %s"), (i + 1), Count, *Message);

				ParseCentrifugeMessage(Message);
			}
		}
		else
		{
			UE_LOG(LogRSCentrifuge, Log, TEXT("Attempting to parse message: %s"), *MessageString);

			ParseCentrifugeMessage(MessageString);
		}
	}

	void FCentrifugeClient::ParseCentrifugeMessage(const FString& MessageString)
	{
		auto Reader = TJsonReaderFactory<>::Create(MessageString);

		TSharedPtr<FJsonValue> JsonValue;
		if (FJsonSerializer::Deserialize(Reader, JsonValue) && JsonValue.IsValid())
		{
			const TSharedPtr<FJsonObject>* JsonObject;
			if (JsonValue->TryGetObject(JsonObject))
			{
				if (TryGetError(*JsonObject) || TryGetReply(*JsonObject) || TryGetPush(*JsonObject))
				{
					UE_LOG(LogRSCentrifuge, Log, TEXT("Successfully converted %s into an ERROR, REPLY, or PUSH message."), *MessageString);
				}
				else
				{
					UE_LOG(LogRSCentrifuge, Error, TEXT("Failed to convert %s into an ERROR, REPLY, or PUSH message."), *MessageString);
				}
			}
			else
			{
				UE_LOG(LogRSCentrifuge, Error, TEXT("Failed to convert %s into a JSON object."), *MessageString);
			}
		}
		else
		{
			UE_LOG(LogRSCentrifuge, Error, TEXT("Failed to convert %s into a JSON value."), *MessageString);
		}
	}

} // namespace RocketScienceMultiplaySDK