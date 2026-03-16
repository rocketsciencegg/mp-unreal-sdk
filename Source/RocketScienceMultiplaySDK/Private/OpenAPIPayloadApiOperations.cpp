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

#include "OpenAPIPayloadApiOperations.h"

#include "RocketScienceMultiplaySDKModule.h"
#include "OpenAPIHelpers.h"

#include "Dom/JsonObject.h"
#include "Templates/SharedPointer.h"
#include "HttpModule.h"
#include "PlatformHttp.h"

namespace RocketScienceMultiplaySDK
{

FString PayloadAllocationRequest::ComputePath() const
{
	TMap<FString, FStringFormatArg> PathParams = { 
	{ TEXT("allocationId"), ToStringFormatArg(AllocationId) } };

	FString Path = FString::Format(TEXT("/payload/{allocationId}"), PathParams);

	return Path;
}

void PayloadAllocationRequest::SetupHttpRequest(const FHttpRequestRef& HttpRequest) const
{
	static const TArray<FString> Consumes = {  };
	//static const TArray<FString> Produces = { TEXT("application/json") };

	HttpRequest->SetVerb(TEXT("GET"));
		
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

void PayloadAllocationResponse::SetHttpResponseCode(EHttpResponseCodes::Type InHttpResponseCode)
{
	Response::SetHttpResponseCode(InHttpResponseCode);
	switch ((int)InHttpResponseCode)
	{
	case 200:
		SetResponseString(TEXT("Response containing the raw bytes of the allocation payload. There are not requirements about the format of the content, this is fully down to the implementation of the game server"));
		break;
	case 400:
		SetResponseString(TEXT("Returned when there&#39;s an internal server error retrieving a payload"));
		break;
	case 500:
		SetResponseString(TEXT("Returned when there&#39;s an internal server error retrieving a payload"));
		break;
	}
}

bool PayloadAllocationResponse::FromJson(const TSharedPtr<FJsonValue>& JsonValue)
{
	return true;
}

FString PayloadTokenRequest::ComputePath() const
{
	FString Path(TEXT("/token"));
	return Path;
}

void PayloadTokenRequest::SetupHttpRequest(const FHttpRequestRef& HttpRequest) const
{
	static const TArray<FString> Consumes = {  };
	//static const TArray<FString> Produces = { TEXT("application/json") };

	HttpRequest->SetVerb(TEXT("GET"));
		
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

void PayloadTokenResponse::SetHttpResponseCode(EHttpResponseCodes::Type InHttpResponseCode)
{
	Response::SetHttpResponseCode(InHttpResponseCode);
	switch ((int)InHttpResponseCode)
	{
	case 200:
		SetResponseString(TEXT("Response returned with a JWT token for payloads"));
		break;
	case 500:
		SetResponseString(TEXT("Response returned with a JWT token for payloads"));
		break;
	}
}

bool PayloadTokenResponse::FromJson(const TSharedPtr<FJsonValue>& JsonValue)
{
	return TryGetJsonValue(JsonValue, Content);
}

}
