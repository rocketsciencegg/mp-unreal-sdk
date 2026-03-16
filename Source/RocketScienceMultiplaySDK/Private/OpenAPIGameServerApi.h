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
class ReadyServerRequest;
class ReadyServerResponse;
class SubscribeServerRequest;
class SubscribeServerResponse;
class UnreadyServerRequest;
class UnreadyServerResponse;

DECLARE_DELEGATE_OneParam(FReadyServerDelegate, const ReadyServerResponse&);
DECLARE_DELEGATE_OneParam(FSubscribeServerDelegate, const SubscribeServerResponse&);
DECLARE_DELEGATE_OneParam(FUnreadyServerDelegate, const UnreadyServerResponse&);

class ROCKETSCIENCEMULTIPLAYSDK_API OpenAPIGameServerApi
{
public:
	OpenAPIGameServerApi();
	~OpenAPIGameServerApi();

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

    FHttpRequestPtr ReadyServer(const ReadyServerRequest& Request, const FReadyServerDelegate& Delegate = FReadyServerDelegate()) const;
    FHttpRequestPtr SubscribeServer(const SubscribeServerRequest& Request, const FSubscribeServerDelegate& Delegate = FSubscribeServerDelegate()) const;
    FHttpRequestPtr UnreadyServer(const UnreadyServerRequest& Request, const FUnreadyServerDelegate& Delegate = FUnreadyServerDelegate()) const;
    
private:
    void OnReadyServerResponse(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded, FReadyServerDelegate Delegate) const;
    void OnSubscribeServerResponse(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded, FSubscribeServerDelegate Delegate) const;
    void OnUnreadyServerResponse(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded, FUnreadyServerDelegate Delegate) const;
    
	FHttpRequestRef CreateHttpRequest(const Request& Request) const;
	bool IsValid() const;
	void HandleResponse(FHttpResponsePtr HttpResponse, bool bSucceeded, Response& InOutResponse) const;

	FString Url;
	TMap<FString,FString> AdditionalHeaderParams;
	mutable FHttpRetrySystem::FManager* RetryManager = nullptr;
	mutable TUniquePtr<HttpRetryManager> DefaultRetryManager;
};

}
