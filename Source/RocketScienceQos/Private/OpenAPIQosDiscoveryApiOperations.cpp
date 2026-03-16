/**
 * QoS Discovery Service API
 * API contract for the server discovery service for QoS servers running as part of the QoS Service.
 *
 * #####################################################################################
 *  Multiplay by Rocket Science Plugin for Unreal Engine Copyright © 2026 Rocket Science Corporation (UK) Ltd.
 * #####################################################################################
 * 
 * NOTE: This is a generated source file. Do not modify manually.
 */

#include "OpenAPIQosDiscoveryApiOperations.h"

#include "RSQosModule.h"
#include "OpenAPIHelpers.h"

#include "Dom/JsonObject.h"
#include "Templates/SharedPointer.h"
#include "HttpModule.h"
#include "PlatformHttp.h"

namespace RocketScienceQos
{

FString ListServersRequest::ComputePath() const
{
	TMap<FString, FStringFormatArg> PathParams = { 
	{ TEXT("fleetid"), ToStringFormatArg(Fleetid) } };

	FString Path = FString::Format(TEXT("/v1/fleets/{fleetid}/servers"), PathParams);

	return Path;
}

void ListServersRequest::SetupHttpRequest(const FHttpRequestRef& HttpRequest) const
{
	static const TArray<FString> Consumes = {  };
	//static const TArray<FString> Produces = { TEXT("application/json") };

	HttpRequest->SetVerb(TEXT("GET"));
		
	TArray<FStringFormatArg> Args = TArray<FStringFormatArg> {ENGINE_MAJOR_VERSION, ENGINE_MINOR_VERSION, ENGINE_PATCH_VERSION};
	FString EngineVersion = FString::Format(TEXT("Engine-{0}.{1}.{2}"), Args);
	HttpRequest->SetHeader(TEXT("User-Agent"), TEXT("X-UnrealEngine-Agent, UGS-Plugin, ") + EngineVersion);

	// Header parameters
	if (IfNoneMatch.IsSet())
	{
		HttpRequest->SetHeader(TEXT("If-None-Match"), IfNoneMatch.GetValue());
	}

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
		UE_LOG(LogRSQos, Error, TEXT("Request ContentType not supported (%s)"), *FString::Join(Consumes, TEXT(",")));
	}
}

void ListServersResponse::SetHttpResponseCode(EHttpResponseCodes::Type InHttpResponseCode)
{
	Response::SetHttpResponseCode(InHttpResponseCode);
	switch ((int)InHttpResponseCode)
	{
	case 200:
		SetResponseString(TEXT("Successful operation"));
		break;
	case 304:
		SetResponseString(TEXT("Not Modified - Content has not changed since the ETag provided in If-None-Match header"));
		break;
	case 403:
		SetResponseString(TEXT("Forbidden"));
		break;
	case 404:
		SetResponseString(TEXT("Fleet not found"));
		break;
	}
}

bool ListServersResponse::FromJson(const TSharedPtr<FJsonValue>& JsonValue)
{
	return TryGetJsonValue(JsonValue, Content);
}

}
