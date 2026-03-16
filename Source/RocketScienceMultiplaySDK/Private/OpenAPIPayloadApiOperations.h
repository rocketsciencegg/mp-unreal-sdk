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

#include "OpenAPIBaseModel.h"
#include "OpenAPIPayloadApi.h"

#include "OpenAPIPayloadAllocationErrorResponseBody.h"
#include "OpenAPIPayloadTokenResponseBody.h"

namespace RocketScienceMultiplaySDK
{

/* Retrieve an allocation&#39;s payload
 *
 * Retrieves the payload associated to a specific allocation
*/
class ROCKETSCIENCEMULTIPLAYSDK_API PayloadAllocationRequest : public Request
{
public:
    virtual ~PayloadAllocationRequest() {}
	void SetupHttpRequest(const FHttpRequestRef& HttpRequest) const final;
	FString ComputePath() const final;

	/* ID of the game server allocation */
	FGuid AllocationId;
};

class ROCKETSCIENCEMULTIPLAYSDK_API PayloadAllocationResponse : public Response
{
public:
    virtual ~PayloadAllocationResponse() {}
	void SetHttpResponseCode(EHttpResponseCodes::Type InHttpResponseCode) final;
	bool FromJson(const TSharedPtr<FJsonValue>& JsonValue) final;

    
};

/* Retrieve a JWT token for payloads
 *
 * Retrieves the JWT token associated to a specific allocation&#39;s fleet
*/
class ROCKETSCIENCEMULTIPLAYSDK_API PayloadTokenRequest : public Request
{
public:
    virtual ~PayloadTokenRequest() {}
	void SetupHttpRequest(const FHttpRequestRef& HttpRequest) const final;
	FString ComputePath() const final;

};

class ROCKETSCIENCEMULTIPLAYSDK_API PayloadTokenResponse : public Response
{
public:
    virtual ~PayloadTokenResponse() {}
	void SetHttpResponseCode(EHttpResponseCodes::Type InHttpResponseCode) final;
	bool FromJson(const TSharedPtr<FJsonValue>& JsonValue) final;

    OpenAPIPayloadTokenResponseBody Content;
};

}
