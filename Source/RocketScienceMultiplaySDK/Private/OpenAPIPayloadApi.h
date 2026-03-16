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

#pragma once

#include "CoreMinimal.h"
#include "OpenAPIBaseModel.h"

namespace RocketScienceMultiplaySDK
{
class PayloadAllocationRequest;
class PayloadAllocationResponse;
class PayloadTokenRequest;
class PayloadTokenResponse;

DECLARE_DELEGATE_OneParam(FPayloadAllocationDelegate, const PayloadAllocationResponse&);
DECLARE_DELEGATE_OneParam(FPayloadTokenDelegate, const PayloadTokenResponse&);

class ROCKETSCIENCEMULTIPLAYSDK_API OpenAPIPayloadApi
{
public:
	OpenAPIPayloadApi();
	~OpenAPIPayloadApi();

	/* Sets the URL Endpoint.
	* Note: several fallback endpoints can be configured in request retry policies, see Request::SetShouldRetry */
	void SetURL(const FString& Url);

	/* Adds global header params to all requests */
	void AddHeaderParam(const FString& Key, const FString& Value);
	void ClearHeaderParams();

	/* Sets the retry manager to the user-defined retry manager. User must manage the lifetime of the retry manager.
	* If no retry manager is specified and a request needs retries, a default retry manager will be used.
	* See also: Request::SetShouldRetry */
	void SetHttpRetryManager(FHttpRetrySystem::FManager& RetryManager);
	FHttpRetrySystem::FManager& GetHttpRetryManager();

    FHttpRequestPtr PayloadAllocation(const PayloadAllocationRequest& Request, const FPayloadAllocationDelegate& Delegate = FPayloadAllocationDelegate()) const;
    FHttpRequestPtr PayloadToken(const PayloadTokenRequest& Request, const FPayloadTokenDelegate& Delegate = FPayloadTokenDelegate()) const;
    
private:
    void OnPayloadAllocationResponse(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded, FPayloadAllocationDelegate Delegate) const;
    void OnPayloadTokenResponse(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded, FPayloadTokenDelegate Delegate) const;
    
	FHttpRequestRef CreateHttpRequest(const Request& Request) const;
	bool IsValid() const;
	void HandleResponse(FHttpResponsePtr HttpResponse, bool bSucceeded, Response& InOutResponse) const;

	FString Url;
	TMap<FString,FString> AdditionalHeaderParams;
	mutable FHttpRetrySystem::FManager* RetryManager = nullptr;
	mutable TUniquePtr<HttpRetryManager> DefaultRetryManager;
};

}
