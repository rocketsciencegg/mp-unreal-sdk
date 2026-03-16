/*
 * #####################################################################################
 *  Rocket Science Multiplay Plugin for Unreal Engine Copyright © 2026 Rocket Science Group
 * #####################################################################################
 */

#include "RSMultiplayGameServerSubsystem.h"
#include "Engine/GameInstance.h"
#include "Subsystems/SubsystemCollection.h"
#include "Centrifuge/MultiplayCentrifugeClient.h"
#include "Centrifuge/MultiplayCentrifugeMessages.h"
#include "MultiplayServerEvents.h"
#include "RSMultiplayServerConfigSubsystem.h"
#include "OpenAPIGameServerApi.h"
#include "OpenAPIGameServerApiOperations.h"
#include "OpenAPIPayloadApi.h"
#include "OpenAPIPayloadApiOperations.h"
#include "OpenAPIErrorResponseBody.h"
#include "OpenAPIPayloadAllocationErrorResponseBody.h"
#include "OpenAPIPayloadTokenResponseBody.h"
#include "MultiplayGameServerSDKLog.h"

 // Necessary to avoid triggering C4150 error for TUniquePtr<FCentrifugeClient> because FCentrifugeClient is forward declared.
 // See documentation in TDefaultDelete<T>::operator() for an explanation.
URSMultiplayGameServerSubsystem::URSMultiplayGameServerSubsystem() = default;
URSMultiplayGameServerSubsystem::~URSMultiplayGameServerSubsystem() = default;
URSMultiplayGameServerSubsystem::URSMultiplayGameServerSubsystem(FVTableHelper& Helper) : URSMultiplaySubsystemBase(Helper) {}

void URSMultiplayGameServerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	// This subsystem is dependent on the server.json file having been parsed so that the serverID value can be retrieved.
	Collection.InitializeDependency(URSMultiplayServerConfigSubsystem::StaticClass());

	FString SdkDaemonIp = TEXT("localhost");
	uint16 SdkDaemonPort = 8086;
	FString SdkDaemonUrl = FString::Printf(TEXT("http://%s:%u"), *SdkDaemonIp, SdkDaemonPort);
	FString SdkDaemonCentrifugeEndpoint = FString::Printf(TEXT("ws://%s:%u/v1/connection/websocket"), *SdkDaemonIp, SdkDaemonPort);

	GameServerApi = MakeUnique<RocketScienceMultiplaySDK::OpenAPIGameServerApi>();
	GameServerApi->SetURL(SdkDaemonUrl);

	PayloadApi = MakeUnique<RocketScienceMultiplaySDK::OpenAPIPayloadApi>();
	PayloadApi->SetURL(SdkDaemonUrl);

	CentrifugeClient = MakeUnique<RocketScienceMultiplaySDK::FCentrifugeClient>(SdkDaemonCentrifugeEndpoint);
	CentrifugeClient->OnConnectReply().AddUObject(this, &URSMultiplayGameServerSubsystem::OnConnectReply);
	CentrifugeClient->OnPublicationPush().AddUObject(this, &URSMultiplayGameServerSubsystem::OnPublicationPush);
}

void URSMultiplayGameServerSubsystem::Deinitialize()
{
	CentrifugeClient->Disconnect();
	CentrifugeClient->OnConnectReply().RemoveAll(this);
	CentrifugeClient->OnPublicationPush().RemoveAll(this);

	Super::Deinitialize();
}

void URSMultiplayGameServerSubsystem::OnConnectReply(const RocketScienceMultiplaySDK::FConnectResult& Result)
{
	UE_LOG(LogRSMultiplayGameServerSDK, Verbose, TEXT("URSMultiplayGameServerSubsystem::OnConnectReply()"));

	URSMultiplayServerConfigSubsystem* Subsystem = GetGameInstance()->GetSubsystem<URSMultiplayServerConfigSubsystem>();
	const FRSMultiplayServerConfig& ServerConfig = Subsystem->GetServerConfig();
	int64 ServerId = ServerConfig.ServerId;

	RocketScienceMultiplaySDK::FSubscribeRequest request = RocketScienceMultiplaySDK::FSubscribeRequest();
	request.Channel = FString::Printf(TEXT("server#%lld"), ServerId);
	CentrifugeClient->Subscribe(request);
}

void URSMultiplayGameServerSubsystem::OnPublicationPush(const RocketScienceMultiplaySDK::FPublication& Push)
{
	UE_LOG(LogRSMultiplayGameServerSDK, Verbose, TEXT("URSMultiplayGameServerSubsystem::OnPublicationPush()"));

	RocketScienceMultiplaySDK::FMultiplayServerAllocateEvent AllocateEvent;
	RocketScienceMultiplaySDK::FMultiplayServerDeallocateEvent DeallocateEvent;

	if (AllocateEvent.FromJson(Push.Data))
	{
		UE_LOG(LogRSMultiplayGameServerSDK, Log, TEXT("Successfully parsed FMultiplayServerAllocateEvent"));

		AllocationId = AllocateEvent.AllocationId;

		FRSMultiplayAllocation MultiplayAllocation;
		MultiplayAllocation.EventId = AllocateEvent.EventId.ToString();
		MultiplayAllocation.ServerId = AllocateEvent.ServerId;
		MultiplayAllocation.AllocationId = AllocateEvent.AllocationId.ToString(EGuidFormats::DigitsWithHyphens).ToLower();

		OnAllocate.Broadcast(MultiplayAllocation);
	}
	else if (DeallocateEvent.FromJson(Push.Data))
	{
		UE_LOG(LogRSMultiplayGameServerSDK, Log, TEXT("Successfully parsed FMultiplayServerDeallocateEvent"));

		AllocationId.Invalidate();

		FRSMultiplayDeallocation MultiplayDeallocation;
		MultiplayDeallocation.EventId = DeallocateEvent.EventId.ToString();
		MultiplayDeallocation.ServerId = DeallocateEvent.ServerId;
		MultiplayDeallocation.AllocationId = DeallocateEvent.AllocationId.ToString(EGuidFormats::DigitsWithHyphens).ToLower();

		OnDeallocate.Broadcast(MultiplayDeallocation);
	}
	else
	{
		UE_LOG(LogRSMultiplayGameServerSDK, Warning, TEXT("Failed to parse PUSH message into an event!"));
	}
}

void URSMultiplayGameServerSubsystem::ReadyServerForPlayers(FRSReadyServerSuccessDelegate OnSuccess, FRSReadyServerFailureDelegate OnFailure)
{
	UE_LOG(LogRSMultiplayGameServerSDK, Verbose, TEXT("URSMultiplayGameServerSubsystem::ReadyServerForPlayers()"));

	OnReadyServerSuccess = OnSuccess;
	OnReadyServerFailure = OnFailure;

	if (AllocationId.IsValid())
	{
		URSMultiplayServerConfigSubsystem* Subsystem = GetGameInstance()->GetSubsystem<URSMultiplayServerConfigSubsystem>();
		const FRSMultiplayServerConfig& ServerConfig = Subsystem->GetServerConfig();
		int64 ServerId = ServerConfig.ServerId;

		RocketScienceMultiplaySDK::ReadyServerRequest Request;
		Request.ServerId = ServerId;
		Request.AllocationId = AllocationId;

		RocketScienceMultiplaySDK::FReadyServerDelegate Delegate =
			RocketScienceMultiplaySDK::FReadyServerDelegate::CreateUObject(this, &URSMultiplayGameServerSubsystem::OnReadyServer);

		GameServerApi->ReadyServer(Request, Delegate);
	}
	else
	{
		FRSMultiplayErrorResponse InvalidAllocationResponse;
		InvalidAllocationResponse.Title = TEXT("Invalid Allocation ID");
		InvalidAllocationResponse.Detail = TEXT("Attempted invoke ReadyServerForPlayers() with an invalid allocation ID.");
		InvalidAllocationResponse.Status = 400;
		OnReadyServerFailure.Execute(InvalidAllocationResponse);
	}
}

void URSMultiplayGameServerSubsystem::UnreadyServer(FRSUnreadyServerSuccessDelegate OnSuccess, FRSUnreadyServerFailureDelegate OnFailure)
{
	UE_LOG(LogRSMultiplayGameServerSDK, Verbose, TEXT("URSMultiplayGameServerSubsystem::UnreadyServer()"));

	OnUnreadyServerSuccess = OnSuccess;
	OnUnreadyServerFailure = OnFailure;

	URSMultiplayServerConfigSubsystem* Subsystem = GetGameInstance()->GetSubsystem<URSMultiplayServerConfigSubsystem>();
	const FRSMultiplayServerConfig& ServerConfig = Subsystem->GetServerConfig();
	int64 ServerId = ServerConfig.ServerId;

	RocketScienceMultiplaySDK::UnreadyServerRequest Request;
	Request.ServerId = ServerId;

	RocketScienceMultiplaySDK::FUnreadyServerDelegate Delegate =
		RocketScienceMultiplaySDK::FUnreadyServerDelegate::CreateUObject(this, &URSMultiplayGameServerSubsystem::OnUnreadyServer);

	GameServerApi->UnreadyServer(Request, Delegate);
}

void URSMultiplayGameServerSubsystem::SubscribeToServerEvents()
{
	UE_LOG(LogRSMultiplayGameServerSDK, Verbose, TEXT("URSMultiplayGameServerSubsystem::SubscribeToServerEvents()"));

	RocketScienceMultiplaySDK::FConnectRequest Request;
	CentrifugeClient->Connect(Request);
}

void URSMultiplayGameServerSubsystem::UnsubscribeToServerEvents()
{
	UE_LOG(LogRSMultiplayGameServerSDK, Verbose, TEXT("URSMultiplayGameServerSubsystem::UnsubscribeToServerEvents()"));

	CentrifugeClient->Disconnect();
}

void URSMultiplayGameServerSubsystem::GetPayloadAllocation(FRSPayloadAllocationSuccessDelegate OnSuccess, FRSPayloadAllocationFailureDelegate OnFailure)
{
	UE_LOG(LogRSMultiplayGameServerSDK, Verbose, TEXT("URSMultiplayGameServerSubsystem::GetPayloadAllocation()"));

	OnPayloadAllocationSuccess = OnSuccess;
	OnPayloadAllocationFailure = OnFailure;

	RocketScienceMultiplaySDK::PayloadAllocationRequest Request;
	Request.AllocationId = AllocationId;

	RocketScienceMultiplaySDK::FPayloadAllocationDelegate Delegate =
		RocketScienceMultiplaySDK::FPayloadAllocationDelegate::CreateUObject(this, &URSMultiplayGameServerSubsystem::OnPayloadAllocation);

	PayloadApi->PayloadAllocation(Request, Delegate);
}

void URSMultiplayGameServerSubsystem::GetPayloadToken(FRSPayloadTokenSuccessDelegate OnSuccess, FRSPayloadTokenFailureDelegate OnFailure)
{
	UE_LOG(LogRSMultiplayGameServerSDK, Verbose, TEXT("URSMultiplayGameServerSubsystem::GetPayloadToken()"));

	OnPayloadTokenSuccess = OnSuccess;
	OnPayloadTokenFailure = OnFailure;

	RocketScienceMultiplaySDK::PayloadTokenRequest Request;

	RocketScienceMultiplaySDK::FPayloadTokenDelegate Delegate =
		RocketScienceMultiplaySDK::FPayloadTokenDelegate::CreateUObject(this, &URSMultiplayGameServerSubsystem::OnPayloadToken);

	PayloadApi->PayloadToken(Request, Delegate);
}

void URSMultiplayGameServerSubsystem::OnReadyServer(const RocketScienceMultiplaySDK::ReadyServerResponse& Response)
{
	if (Response.IsSuccessful())
	{
		UE_LOG(LogRSMultiplayGameServerSDK, Log, TEXT("ServerReady() was successful"));
		OnReadyServerSuccess.ExecuteIfBound();
	}
	else
	{
		const FHttpResponsePtr& HttpResponse = Response.GetHttpResponse();
		FRSMultiplayErrorResponse MultiplayErrorResponseBodyStruct = {};

		if (!HttpResponse)
		{
			MultiplayErrorResponseBodyStruct.Status = static_cast<int32>(Response.GetHttpResponseCode());
			MultiplayErrorResponseBodyStruct.Detail = TEXT("HttpResponse is NULL");
			MultiplayErrorResponseBodyStruct.Title = TEXT("Parsing failed error");

			UE_LOG(LogRSMultiplayGameServerSDK, Error, TEXT("OnReadyServer() was unsuccessful, response status code is '%d' message '%s'"), MultiplayErrorResponseBodyStruct.Status, *MultiplayErrorResponseBodyStruct.Detail);

			OnReadyServerFailure.ExecuteIfBound(MultiplayErrorResponseBodyStruct);

			return;
		}

		int32 ResponseCode = HttpResponse->GetResponseCode();
		FString ResponseBody = HttpResponse->GetContentAsString();
		TSharedPtr<FJsonValue> JsonParsed;
		TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<TCHAR>::Create(ResponseBody);

		if (FJsonSerializer::Deserialize(JsonReader, JsonParsed) && JsonParsed.IsValid())
		{
			TUniquePtr<RocketScienceMultiplaySDK::OpenAPIErrorResponseBody> ErrorResponseBody = MakeUnique<RocketScienceMultiplaySDK::OpenAPIErrorResponseBody>();
			if (ErrorResponseBody->FromJson(JsonParsed))
			{
				MultiplayErrorResponseBodyStruct.Status = ErrorResponseBody->Status;
				MultiplayErrorResponseBodyStruct.Detail = ErrorResponseBody->Detail;
				MultiplayErrorResponseBodyStruct.Title = ErrorResponseBody->Title;
			}
			else
			{
				MultiplayErrorResponseBodyStruct.Status = 500;
				MultiplayErrorResponseBodyStruct.Detail = TEXT("Failed to deserialize ReadyServer JSON response body on failure");
				MultiplayErrorResponseBodyStruct.Title = TEXT("Parsing failed error");
			}
		}
		else
		{
			MultiplayErrorResponseBodyStruct.Status = 500;
			MultiplayErrorResponseBodyStruct.Detail = TEXT("Failed to deserialize ReadyServer JSON response body on failure");
			MultiplayErrorResponseBodyStruct.Title = TEXT("Parsing failed error");
		}

		UE_LOG(LogRSMultiplayGameServerSDK, Error, TEXT("ServerReady() was unsuccessful, response status code is '%d' and response body is '%s'"), ResponseCode, *ResponseBody);

		OnReadyServerFailure.ExecuteIfBound(MultiplayErrorResponseBodyStruct);
	}
}

void URSMultiplayGameServerSubsystem::OnUnreadyServer(const RocketScienceMultiplaySDK::UnreadyServerResponse& Response)
{
	if (Response.IsSuccessful())
	{
		UE_LOG(LogRSMultiplayGameServerSDK, Log, TEXT("ServerUnready() was successful"));

		OnUnreadyServerSuccess.ExecuteIfBound();
	}
	else
	{
		const FHttpResponsePtr& HttpResponse = Response.GetHttpResponse();

		FRSMultiplayErrorResponse MultiplayErrorResponseBodyStruct = {};

		if (!HttpResponse)
		{
			MultiplayErrorResponseBodyStruct.Status = static_cast<int32>(Response.GetHttpResponseCode());
			MultiplayErrorResponseBodyStruct.Detail = TEXT("HttpResponse is NULL");
			MultiplayErrorResponseBodyStruct.Title = TEXT("Parsing failed error");

			UE_LOG(LogRSMultiplayGameServerSDK, Error, TEXT("OnReadyServer() was unsuccessful, response status code is '%d' message '%s'"), MultiplayErrorResponseBodyStruct.Status, *MultiplayErrorResponseBodyStruct.Detail);

			OnReadyServerFailure.ExecuteIfBound(MultiplayErrorResponseBodyStruct);

			return;
		}

		int32 ResponseCode = HttpResponse->GetResponseCode();
		FString ResponseBody = HttpResponse->GetContentAsString();
		TSharedPtr<FJsonValue> JsonParsed;
		TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<TCHAR>::Create(ResponseBody);

		if (FJsonSerializer::Deserialize(JsonReader, JsonParsed) && JsonParsed.IsValid())
		{
			TUniquePtr<RocketScienceMultiplaySDK::OpenAPIErrorResponseBody> ErrorResponseBody = MakeUnique<RocketScienceMultiplaySDK::OpenAPIErrorResponseBody>();
			if (ErrorResponseBody->FromJson(JsonParsed))
			{
				MultiplayErrorResponseBodyStruct.Status = ErrorResponseBody->Status;
				MultiplayErrorResponseBodyStruct.Detail = ErrorResponseBody->Detail;
				MultiplayErrorResponseBodyStruct.Title = ErrorResponseBody->Title;
			}
			else
			{
				MultiplayErrorResponseBodyStruct.Status = 500;
				MultiplayErrorResponseBodyStruct.Detail = TEXT("Failed to deserialize ReadyServer JSON response body on failure");
				MultiplayErrorResponseBodyStruct.Title = TEXT("Parsing failed error");
			}
		}
		else
		{
			MultiplayErrorResponseBodyStruct.Status = 500;
			MultiplayErrorResponseBodyStruct.Detail = TEXT("Failed to deserialize ReadyServer JSON response body on failure");
			MultiplayErrorResponseBodyStruct.Title = TEXT("Parsing failed error");
		}

		UE_LOG(LogRSMultiplayGameServerSDK, Error, TEXT("OnUnreadyServer() was unsuccessful, response status code is '%d' and response body is '%s'"), ResponseCode, *ResponseBody);

		OnUnreadyServerFailure.ExecuteIfBound(MultiplayErrorResponseBodyStruct);
	}
}

void URSMultiplayGameServerSubsystem::OnPayloadAllocation(const RocketScienceMultiplaySDK::PayloadAllocationResponse& Response)
{
	FRSMultiplayPayloadAllocationErrorResponse MultiplayErrorResponseBodyStruct = {};
	if (Response.IsSuccessful())
	{
		UE_LOG(LogRSMultiplayGameServerSDK, Log, TEXT("PayloadAllocation() was successful"));
		const FHttpResponsePtr& HttpResponse = Response.GetHttpResponse();

		if (!HttpResponse)
		{
			MultiplayErrorResponseBodyStruct.Error = true;
			MultiplayErrorResponseBodyStruct.ErrorCode = static_cast<int32>(Response.GetHttpResponseCode());
			MultiplayErrorResponseBodyStruct.ErrorMessage = TEXT("HttpResponse is NULL");
			MultiplayErrorResponseBodyStruct.Success = false;

			UE_LOG(LogRSMultiplayGameServerSDK, Error, TEXT("OnPayloadAllocation() was successful, but error response status code is '%d' message '%s'"), MultiplayErrorResponseBodyStruct.ErrorCode, *MultiplayErrorResponseBodyStruct.ErrorMessage);

			OnPayloadAllocationFailure.ExecuteIfBound(MultiplayErrorResponseBodyStruct);

			return;
		}

		FString ResponseBody = HttpResponse->GetContentAsString();
		OnPayloadAllocationSuccess.ExecuteIfBound(ResponseBody);
	}
	else
	{
		const FHttpResponsePtr& HttpResponse = Response.GetHttpResponse();

		if (!HttpResponse)
		{
			MultiplayErrorResponseBodyStruct.Error = true;
			MultiplayErrorResponseBodyStruct.ErrorCode = static_cast<int32>(Response.GetHttpResponseCode());
			MultiplayErrorResponseBodyStruct.ErrorMessage = TEXT("HttpResponse is NULL");
			MultiplayErrorResponseBodyStruct.Success = false;

			UE_LOG(LogRSMultiplayGameServerSDK, Error, TEXT("OnPayloadAllocation() was unsuccessful, response status code is '%d' message '%s'"), MultiplayErrorResponseBodyStruct.ErrorCode, *MultiplayErrorResponseBodyStruct.ErrorMessage);

			OnPayloadAllocationFailure.ExecuteIfBound(MultiplayErrorResponseBodyStruct);

			return;
		}

		int32 ResponseCode = HttpResponse->GetResponseCode();
		FString ResponseBody = HttpResponse->GetContentAsString();
		TSharedPtr<FJsonValue> JsonParsed;
		TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<TCHAR>::Create(ResponseBody);

		if (FJsonSerializer::Deserialize(JsonReader, JsonParsed) && JsonParsed.IsValid())
		{
			TUniquePtr<RocketScienceMultiplaySDK::OpenAPIPayloadAllocationErrorResponseBody> ErrorResponseBody = MakeUnique<RocketScienceMultiplaySDK::OpenAPIPayloadAllocationErrorResponseBody>();
			if (ErrorResponseBody->FromJson(JsonParsed))
			{
				MultiplayErrorResponseBodyStruct.Error = ErrorResponseBody->Error;
				MultiplayErrorResponseBodyStruct.ErrorCode = ErrorResponseBody->ErrorCode;
				MultiplayErrorResponseBodyStruct.ErrorMessage = ErrorResponseBody->ErrorMessage;
				MultiplayErrorResponseBodyStruct.Success = ErrorResponseBody->Success;
			}
			else
			{
				MultiplayErrorResponseBodyStruct.Error = true;
				MultiplayErrorResponseBodyStruct.ErrorCode = 500;
				MultiplayErrorResponseBodyStruct.ErrorMessage = TEXT("Failed to deserialize JSON response body on failure");
				MultiplayErrorResponseBodyStruct.Success = false;
			}
		}
		else
		{
			MultiplayErrorResponseBodyStruct.Error = true;
			MultiplayErrorResponseBodyStruct.ErrorCode = 500;
			MultiplayErrorResponseBodyStruct.ErrorMessage = TEXT("Failed to deserialize JSON response body on failure");
			MultiplayErrorResponseBodyStruct.Success = false;
		}

		UE_LOG(LogRSMultiplayGameServerSDK, Error, TEXT("OnPayloadAllocation() was unsuccessful, response status code is '%d' and response body is '%s'"), ResponseCode, *ResponseBody);

		OnPayloadAllocationFailure.ExecuteIfBound(MultiplayErrorResponseBodyStruct);
	}
}

void URSMultiplayGameServerSubsystem::OnPayloadToken(const RocketScienceMultiplaySDK::PayloadTokenResponse& Response)
{
	FRSMultiplayPayloadTokenResponse MultiplayTokenResponseBodyStruct = {};

	if (Response.IsSuccessful())
	{
		UE_LOG(LogRSMultiplayGameServerSDK, Log, TEXT("PayloadToken() was successful"));
		const FHttpResponsePtr& HttpResponse = Response.GetHttpResponse();

		if (!HttpResponse)
		{
			MultiplayTokenResponseBodyStruct.Token = TEXT("");
			MultiplayTokenResponseBodyStruct.Error = TEXT("HttpResponse is NULL");

			UE_LOG(LogRSMultiplayGameServerSDK, Error, TEXT("OnPayloadToken() was successful, but the response was NULL - message '%s'"), *MultiplayTokenResponseBodyStruct.Error);

			OnPayloadTokenFailure.ExecuteIfBound(MultiplayTokenResponseBodyStruct);

			return;
		}

		int32 ResponseCode = HttpResponse->GetResponseCode();
		FString ResponseBody = HttpResponse->GetContentAsString();
		TSharedPtr<FJsonValue> JsonParsed;
		TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<TCHAR>::Create(ResponseBody);

		if (FJsonSerializer::Deserialize(JsonReader, JsonParsed) && JsonParsed.IsValid())
		{
			TUniquePtr<RocketScienceMultiplaySDK::OpenAPIPayloadTokenResponseBody> PayloadTokenResponseBody = MakeUnique<RocketScienceMultiplaySDK::OpenAPIPayloadTokenResponseBody>();
			if (PayloadTokenResponseBody->FromJson(JsonParsed))
			{
				MultiplayTokenResponseBodyStruct.Error = PayloadTokenResponseBody->Error;
				MultiplayTokenResponseBodyStruct.Token = PayloadTokenResponseBody->Token;
				OnPayloadTokenSuccess.ExecuteIfBound(MultiplayTokenResponseBodyStruct);
			}
			else
			{
				MultiplayTokenResponseBodyStruct.Error = TEXT("Succeeded retrieving token but failed to deserialize the response");
				MultiplayTokenResponseBodyStruct.Token = TEXT("");
				OnPayloadTokenFailure.ExecuteIfBound(MultiplayTokenResponseBodyStruct);
			}
		}
		else
		{
			MultiplayTokenResponseBodyStruct.Error = TEXT("Succeeded retrieving token but failed to deserialize the response");
			MultiplayTokenResponseBodyStruct.Token = TEXT("");
			OnPayloadTokenFailure.ExecuteIfBound(MultiplayTokenResponseBodyStruct);
		}
	}
	else
	{
		const FHttpResponsePtr& HttpResponse = Response.GetHttpResponse();

		if (!HttpResponse)
		{
			MultiplayTokenResponseBodyStruct.Token = TEXT("");
			MultiplayTokenResponseBodyStruct.Error = TEXT("HttpResponse is NULL");

			UE_LOG(LogRSMultiplayGameServerSDK, Error, TEXT("OnPayloadToken() was unsuccessful, message '%s'"), *MultiplayTokenResponseBodyStruct.Error);

			OnPayloadTokenFailure.ExecuteIfBound(MultiplayTokenResponseBodyStruct);

			return;
		}

		int32 ResponseCode = HttpResponse->GetResponseCode();
		FString ResponseBody = HttpResponse->GetContentAsString();
		TSharedPtr<FJsonValue> JsonParsed;
		TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<TCHAR>::Create(ResponseBody);

		if (FJsonSerializer::Deserialize(JsonReader, JsonParsed) && JsonParsed.IsValid())
		{
			TUniquePtr<RocketScienceMultiplaySDK::OpenAPIPayloadTokenResponseBody> ErrorResponseBody = MakeUnique<RocketScienceMultiplaySDK::OpenAPIPayloadTokenResponseBody>();
			if (ErrorResponseBody->FromJson(JsonParsed))
			{
				MultiplayTokenResponseBodyStruct.Error = ErrorResponseBody->Error;
				MultiplayTokenResponseBodyStruct.Token = ErrorResponseBody->Token;
			}
			else
			{
				MultiplayTokenResponseBodyStruct.Error = TEXT("Failed to deserialize Json response body on failure");
				MultiplayTokenResponseBodyStruct.Token = TEXT("");
			}
		}
		else
		{
			MultiplayTokenResponseBodyStruct.Error = TEXT("Failed to deserialize Json response body on failure");
			MultiplayTokenResponseBodyStruct.Token = TEXT("");
		}

		UE_LOG(LogRSMultiplayGameServerSDK, Error, TEXT("OnPayloadToken() was unsuccessful, response status code is '%d' and response body is '%s'"), ResponseCode, *ResponseBody);

		OnPayloadTokenFailure.ExecuteIfBound(MultiplayTokenResponseBodyStruct);
	}
}