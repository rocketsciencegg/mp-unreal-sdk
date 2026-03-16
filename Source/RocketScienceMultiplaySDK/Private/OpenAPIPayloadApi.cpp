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

#include "OpenAPIPayloadApi.h"

#include "OpenAPIPayloadApiOperations.h"
#include "RocketScienceMultiplaySDKModule.h"

#include "HttpModule.h"
#include "Serialization/JsonSerializer.h"

namespace RocketScienceMultiplaySDK
{

OpenAPIPayloadApi::OpenAPIPayloadApi()
: Url(TEXT("http://localhost"))
{
}

OpenAPIPayloadApi::~OpenAPIPayloadApi() {}

void OpenAPIPayloadApi::SetURL(const FString& InUrl)
{
	Url = InUrl;
}

void OpenAPIPayloadApi::AddHeaderParam(const FString& Key, const FString& Value)
{
	AdditionalHeaderParams.Add(Key, Value);
}

void OpenAPIPayloadApi::ClearHeaderParams()
{
	AdditionalHeaderParams.Reset();
}

bool OpenAPIPayloadApi::IsValid() const
{
	if (Url.IsEmpty())
	{
		UE_LOG(LogRSMultiplayGameServerSDK, Error, TEXT("OpenAPIPayloadApi: Endpoint Url is not set, request cannot be performed"));
		return false;
	}

	return true;
}

void OpenAPIPayloadApi::SetHttpRetryManager(FHttpRetrySystem::FManager& InRetryManager)
{
	if(RetryManager != &GetHttpRetryManager())
	{
		DefaultRetryManager.Reset();
		RetryManager = &InRetryManager;
	}
}

FHttpRetrySystem::FManager& OpenAPIPayloadApi::GetHttpRetryManager()
{
	checkf(RetryManager, TEXT("OpenAPIPayloadApi: RetryManager is null.  You may have meant to set it with SetHttpRetryManager first, or you may not be using a custom RetryManager at all."))
	return *RetryManager;
}

FHttpRequestRef OpenAPIPayloadApi::CreateHttpRequest(const Request& Request) const
{
	if (!Request.GetRetryParams().IsSet())
	{
		return FHttpModule::Get().CreateRequest();
	}
	else
	{
		if (!RetryManager)
		{
			// Create default retry manager if none was specified
			DefaultRetryManager = MakeUnique<HttpRetryManager>(6, 60);
			RetryManager = DefaultRetryManager.Get();
		}

		const HttpRetryParams& Params = Request.GetRetryParams().GetValue();
		return RetryManager->CreateRequest(Params.RetryLimitCountOverride, Params.RetryTimeoutRelativeSecondsOverride, Params.RetryResponseCodes, Params.RetryVerbs, Params.RetryDomains);
	}
}

void OpenAPIPayloadApi::HandleResponse(FHttpResponsePtr HttpResponse, bool bSucceeded, Response& InOutResponse) const
{
	InOutResponse.SetHttpResponse(HttpResponse);
	InOutResponse.SetSuccessful(bSucceeded);

	if (bSucceeded && HttpResponse.IsValid())
	{
		InOutResponse.SetHttpResponseCode((EHttpResponseCodes::Type)HttpResponse->GetResponseCode());
		FString ContentType = HttpResponse->GetContentType();
		FString Content;

		if (ContentType.IsEmpty())
		{
			return; // Nothing to parse
		}
		else if (ContentType.StartsWith(TEXT("application/json")) || ContentType.StartsWith("text/json"))
		{
			Content = HttpResponse->GetContentAsString();

			// This is being added as Unitys Matchmaker API returns empty responses as both 200 and 204. 
			// They also contain a application/json Content-Type which falls into this case but fails to parse as it is empty. 
			// Implementing this as a workaround to prevent error logging.
			if (Content.Len() == 0)
			{
				return; // Nothing to parse
			}

			TSharedPtr<FJsonValue> JsonValue;
			auto Reader = TJsonReaderFactory<>::Create(Content);

			if (FJsonSerializer::Deserialize(Reader, JsonValue) && JsonValue.IsValid())
			{
				if (InOutResponse.FromJson(JsonValue))
					return; // Successfully parsed
			}
		}
		else if(ContentType.StartsWith(TEXT("text/plain")))
		{
			Content = HttpResponse->GetContentAsString();
			InOutResponse.SetResponseString(Content);
			return; // Successfully parsed
		}
		else if (ContentType.StartsWith(TEXT("application/problem+json")))
		{
			Content = HttpResponse->GetContentAsString();
			UE_LOG(LogRSMultiplayGameServerSDK, Error, TEXT("OpenAPIPayloadApi: Request returned error code %d from remote endpoint."), HttpResponse->GetResponseCode());
			UE_LOG(LogRSMultiplayGameServerSDK, Error, TEXT("OpenAPIPayloadApi: %s"), *Content);
			InOutResponse.SetSuccessful(false);
			return; // Error message successfully parsed
		}

		// Report the parse error but do not mark the request as unsuccessful. Data could be partial or malformed, but the request succeeded.
		UE_LOG(LogRSMultiplayGameServerSDK, Error, TEXT("Failed to deserialize Http response content (type:%s):\n%s"), *ContentType , *Content);
		return;
	}

	// By default, assume we failed to establish connection
	InOutResponse.SetHttpResponseCode(EHttpResponseCodes::RequestTimeout);
}

FHttpRequestPtr OpenAPIPayloadApi::PayloadAllocation(const PayloadAllocationRequest& Request, const FPayloadAllocationDelegate& Delegate /*= FPayloadAllocationDelegate()*/) const
{
	if (!IsValid())
		return nullptr;

	FHttpRequestRef HttpRequest = CreateHttpRequest(Request);
	HttpRequest->SetURL(*(Url + Request.ComputePath()));

	for(const auto& It : AdditionalHeaderParams)
	{
		HttpRequest->SetHeader(It.Key, It.Value);
	}

	Request.SetupHttpRequest(HttpRequest);

	HttpRequest->OnProcessRequestComplete().BindRaw(this, &OpenAPIPayloadApi::OnPayloadAllocationResponse, Delegate);
	HttpRequest->ProcessRequest();
	return HttpRequest;
}

void OpenAPIPayloadApi::OnPayloadAllocationResponse(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded, FPayloadAllocationDelegate Delegate) const
{
	PayloadAllocationResponse Response;
	HandleResponse(HttpResponse, bSucceeded, Response);
	Delegate.ExecuteIfBound(Response);
}

FHttpRequestPtr OpenAPIPayloadApi::PayloadToken(const PayloadTokenRequest& Request, const FPayloadTokenDelegate& Delegate /*= FPayloadTokenDelegate()*/) const
{
	if (!IsValid())
		return nullptr;

	FHttpRequestRef HttpRequest = CreateHttpRequest(Request);
	HttpRequest->SetURL(*(Url + Request.ComputePath()));

	for(const auto& It : AdditionalHeaderParams)
	{
		HttpRequest->SetHeader(It.Key, It.Value);
	}

	Request.SetupHttpRequest(HttpRequest);

	HttpRequest->OnProcessRequestComplete().BindRaw(this, &OpenAPIPayloadApi::OnPayloadTokenResponse, Delegate);
	HttpRequest->ProcessRequest();
	return HttpRequest;
}

void OpenAPIPayloadApi::OnPayloadTokenResponse(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded, FPayloadTokenDelegate Delegate) const
{
	PayloadTokenResponse Response;
	HandleResponse(HttpResponse, bSucceeded, Response);
	Delegate.ExecuteIfBound(Response);
}

}
