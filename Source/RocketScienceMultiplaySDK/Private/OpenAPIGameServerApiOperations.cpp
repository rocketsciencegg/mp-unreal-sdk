/**
 * Multiplay by Rocket Science SDK Daemon Game Server API
 * The game server API is called by the Multiplay Game Server SDK itself to subscribe to events and report server status.
 *
 * #####################################################################################
 *  Multiplay by Rocket Science Plugin for Unreal Engine Copyright © 2026 Rocket Science Corporation (UK) Ltd.
 * #####################################################################################
 * 
 * NOTE: This is a generated source file. Do not modify manually.
 */

#include "OpenAPIGameServerApiOperations.h"

#include "RocketScienceMultiplaySDKModule.h"
#include "OpenAPIHelpers.h"

#include "Dom/JsonObject.h"
#include "Templates/SharedPointer.h"
#include "HttpModule.h"
#include "PlatformHttp.h"

namespace RocketScienceMultiplaySDK
{

FString ReadyServerRequest::ComputePath() const
{
	TMap<FString, FStringFormatArg> PathParams = { 
	{ TEXT("serverId"), ToStringFormatArg(ServerId) },
	{ TEXT("allocationId"), ToStringFormatArg(AllocationId) } };

	FString Path = FString::Format(TEXT("/v1/server/{serverId}/allocation/{allocationId}/ready-for-players"), PathParams);

	return Path;
}

void ReadyServerRequest::SetupHttpRequest(const FHttpRequestRef& HttpRequest) const
{
	static const TArray<FString> Consumes = {  };
	//static const TArray<FString> Produces = { TEXT("application/json"), TEXT("application/problem+json") };

	HttpRequest->SetVerb(TEXT("POST"));
		
	TArray<FStringFormatArg> Args = TArray<FStringFormatArg> {ENGINE_MAJOR_VERSION, ENGINE_MINOR_VERSION, ENGINE_PATCH_VERSION};
	FString EngineVersion = FString::Format(TEXT("Engine-{0}.{1}.{2}"), Args);
	HttpRequest->SetHeader(TEXT("User-Agent"), TEXT("X-UnrealEngine-Agent, UGS-Plugin, ") + EngineVersion);

	// Default to Json Body request
	if (Consumes.Contains(TEXT("application/json")))
	{
		// Form parameters
		FString JsonBody;
		JsonWriter Writer = TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&JsonBody);
		Writer->WriteObjectStart();
		Writer->WriteObjectEnd();
		Writer->Close();
		
		HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/json; charset=utf-8"));
		HttpRequest->SetContentAsString(JsonBody);
	}
	else if (Consumes.Contains(TEXT("multipart/form-data")))
	{
	}
	else if (Consumes.Contains(TEXT("application/x-www-form-urlencoded")))
	{
	}
	else if (Consumes.Num() == 0)
	{
		// Do nothing.
	}
	else
	{
		UE_LOG(LogRSMultiplayGameServerSDK, Error, TEXT("Request ContentType not supported (%s)"), *FString::Join(Consumes, TEXT(",")));
	}
}

void ReadyServerResponse::SetHttpResponseCode(EHttpResponseCodes::Type InHttpResponseCode)
{
	Response::SetHttpResponseCode(InHttpResponseCode);
	switch ((int)InHttpResponseCode)
	{
	case 200:
		SetResponseString(TEXT("Response returned when a server has been marked as ready to receive players"));
		break;
	case 404:
		SetResponseString(TEXT("RFC7807 response returned in the event of an error."));
		break;
	case 500:
		SetResponseString(TEXT("RFC7807 response returned in the event of an error."));
		break;
	}
}

bool ReadyServerResponse::FromJson(const TSharedPtr<FJsonValue>& JsonValue)
{
	return TryGetJsonValue(JsonValue, Content);
}

FString SubscribeServerRequest::ComputePath() const
{
	FString Path(TEXT("/v1/connection/websocket"));
	return Path;
}

void SubscribeServerRequest::SetupHttpRequest(const FHttpRequestRef& HttpRequest) const
{
	static const TArray<FString> Consumes = {  };
	//static const TArray<FString> Produces = { TEXT("application/problem+json") };

	HttpRequest->SetVerb(TEXT("GET"));
		
	TArray<FStringFormatArg> Args = TArray<FStringFormatArg> {ENGINE_MAJOR_VERSION, ENGINE_MINOR_VERSION, ENGINE_PATCH_VERSION};
	FString EngineVersion = FString::Format(TEXT("Engine-{0}.{1}.{2}"), Args);
	HttpRequest->SetHeader(TEXT("User-Agent"), TEXT("X-UnrealEngine-Agent, UGS-Plugin, ") + EngineVersion);

	// Header parameters
	HttpRequest->SetHeader(TEXT("Connection"), Connection);
	HttpRequest->SetHeader(TEXT("Upgrade"), Upgrade);

	// Default to Json Body request
	if (Consumes.Contains(TEXT("application/json")))
	{
		// Form parameters
		FString JsonBody;
		JsonWriter Writer = TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&JsonBody);
		Writer->WriteObjectStart();
		Writer->WriteObjectEnd();
		Writer->Close();
		
		HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/json; charset=utf-8"));
		HttpRequest->SetContentAsString(JsonBody);
	}
	else if (Consumes.Contains(TEXT("multipart/form-data")))
	{
	}
	else if (Consumes.Contains(TEXT("application/x-www-form-urlencoded")))
	{
	}
	else if (Consumes.Num() == 0)
	{
		// Do nothing.
	}
	else
	{
		UE_LOG(LogRSMultiplayGameServerSDK, Error, TEXT("Request ContentType not supported (%s)"), *FString::Join(Consumes, TEXT(",")));
	}
}

void SubscribeServerResponse::SetHttpResponseCode(EHttpResponseCodes::Type InHttpResponseCode)
{
	Response::SetHttpResponseCode(InHttpResponseCode);
	switch ((int)InHttpResponseCode)
	{
	case 101:
		SetResponseString(TEXT("Returned when upgrading to a websocket connection for server events"));
		break;
	case 400:
		SetResponseString(TEXT("RFC7807 response returned in the event of an error."));
		break;
	case 404:
		SetResponseString(TEXT("RFC7807 response returned in the event of an error."));
		break;
	case 500:
		SetResponseString(TEXT("RFC7807 response returned in the event of an error."));
		break;
	}
}

bool SubscribeServerResponse::FromJson(const TSharedPtr<FJsonValue>& JsonValue)
{
	return true;
}

FString UnreadyServerRequest::ComputePath() const
{
	TMap<FString, FStringFormatArg> PathParams = { 
	{ TEXT("serverId"), ToStringFormatArg(ServerId) } };

	FString Path = FString::Format(TEXT("/v1/server/{serverId}/unready"), PathParams);

	return Path;
}

void UnreadyServerRequest::SetupHttpRequest(const FHttpRequestRef& HttpRequest) const
{
	static const TArray<FString> Consumes = {  };
	//static const TArray<FString> Produces = { TEXT("application/json"), TEXT("application/problem+json") };

	HttpRequest->SetVerb(TEXT("POST"));
		
	TArray<FStringFormatArg> Args = TArray<FStringFormatArg> {ENGINE_MAJOR_VERSION, ENGINE_MINOR_VERSION, ENGINE_PATCH_VERSION};
	FString EngineVersion = FString::Format(TEXT("Engine-{0}.{1}.{2}"), Args);
	HttpRequest->SetHeader(TEXT("User-Agent"), TEXT("X-UnrealEngine-Agent, UGS-Plugin, ") + EngineVersion);

	// Default to Json Body request
	if (Consumes.Contains(TEXT("application/json")))
	{
		// Form parameters
		FString JsonBody;
		JsonWriter Writer = TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&JsonBody);
		Writer->WriteObjectStart();
		Writer->WriteObjectEnd();
		Writer->Close();
		
		HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/json; charset=utf-8"));
		HttpRequest->SetContentAsString(JsonBody);
	}
	else if (Consumes.Contains(TEXT("multipart/form-data")))
	{
	}
	else if (Consumes.Contains(TEXT("application/x-www-form-urlencoded")))
	{
	}
	else if (Consumes.Num() == 0)
	{
		// Do nothing.
	}
	else
	{
		UE_LOG(LogRSMultiplayGameServerSDK, Error, TEXT("Request ContentType not supported (%s)"), *FString::Join(Consumes, TEXT(",")));
	}
}

void UnreadyServerResponse::SetHttpResponseCode(EHttpResponseCodes::Type InHttpResponseCode)
{
	Response::SetHttpResponseCode(InHttpResponseCode);
	switch ((int)InHttpResponseCode)
	{
	case 200:
		SetResponseString(TEXT("Response returned when a server has been marked as unready to receive players"));
		break;
	case 404:
		SetResponseString(TEXT("RFC7807 response returned in the event of an error."));
		break;
	case 500:
		SetResponseString(TEXT("RFC7807 response returned in the event of an error."));
		break;
	}
}

bool UnreadyServerResponse::FromJson(const TSharedPtr<FJsonValue>& JsonValue)
{
	return TryGetJsonValue(JsonValue, Content);
}

}
