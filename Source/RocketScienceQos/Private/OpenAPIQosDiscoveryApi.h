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

#pragma once

#include "CoreMinimal.h"
#include "OpenAPIBaseModel.h"

namespace RocketScienceQos
{
class ListServersRequest;
class ListServersResponse;

DECLARE_DELEGATE_OneParam(FListServersDelegate, const ListServersResponse&);

class ROCKETSCIENCEQOS_API OpenAPIQosDiscoveryApi
{
public:
	OpenAPIQosDiscoveryApi();
	~OpenAPIQosDiscoveryApi();

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

    FHttpRequestPtr ListServers(const ListServersRequest& Request, const FListServersDelegate& Delegate = FListServersDelegate()) const;
    
private:
    void OnListServersResponse(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded, FListServersDelegate Delegate) const;
    
	FHttpRequestRef CreateHttpRequest(const Request& Request) const;
	bool IsValid() const;
	void HandleResponse(FHttpResponsePtr HttpResponse, bool bSucceeded, Response& InOutResponse) const;

	FString Url;
	TMap<FString,FString> AdditionalHeaderParams;
	mutable FHttpRetrySystem::FManager* RetryManager = nullptr;
	mutable TUniquePtr<HttpRetryManager> DefaultRetryManager;
};

}
