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
#include "OpenAPIGameServerApi.h"

#include "OpenAPIErrorResponseBody.h"

namespace RocketScienceMultiplaySDK
{

/* Indicates a server is ready to receive players
 *
 * Called by the Game Server SDK (or customer code if SDK is not used) to indicate that it is ready to receive players.
*/
class ROCKETSCIENCEMULTIPLAYSDK_API ReadyServerRequest : public Request
{
public:
    virtual ~ReadyServerRequest() {}
	void SetupHttpRequest(const FHttpRequestRef& HttpRequest) const final;
	FString ComputePath() const final;

	/* ID of the game server */
	int64 ServerId = 0;
	/* ID of the game server allocation */
	FGuid AllocationId;
};

class ROCKETSCIENCEMULTIPLAYSDK_API ReadyServerResponse : public Response
{
public:
    virtual ~ReadyServerResponse() {}
	void SetHttpResponseCode(EHttpResponseCodes::Type InHttpResponseCode) final;
	bool FromJson(const TSharedPtr<FJsonValue>& JsonValue) final;

    TSharedPtr<FJsonObject> Content;
};

/* Subscribe to game server lifecycle events
 *
 * Subscribe to lifecycle events for a given game server. This is a websocket upgrade using the Centrifuge (https://github.com/centrifugal/centrifuge) protocol.  Upon calling this endpoint with a Centrifuge client, the connection will be upgraded to a single-direction stream from the SDK Daemon to the caller. The WebSocket connection is implemented using the Centrifuge real time messaging library. The client must be compliant with the Centrifuge protocol, a vanilla WebSocket client is not sufficient.  The client is expected to use this connection to then subscribe to a channel named in the format &#x60;server#&lt;serverid&gt;&#x60; where &#x60;&lt;serverid&gt;&#x60; is the unique 64-bit integer identifier for this server. 
*/
class ROCKETSCIENCEMULTIPLAYSDK_API SubscribeServerRequest : public Request
{
public:
    virtual ~SubscribeServerRequest() {}
	void SetupHttpRequest(const FHttpRequestRef& HttpRequest) const final;
	FString ComputePath() const final;

	/* Controls whether the network connection stays open after the current transaction finishes. */
	FString Connection;
	/* Used to upgrade an already established client/server connection to a different protocol */
	FString Upgrade;
};

class ROCKETSCIENCEMULTIPLAYSDK_API SubscribeServerResponse : public Response
{
public:
    virtual ~SubscribeServerResponse() {}
	void SetHttpResponseCode(EHttpResponseCodes::Type InHttpResponseCode) final;
	bool FromJson(const TSharedPtr<FJsonValue>& JsonValue) final;

    
};

/* Indicates a server is not ready to receive allocations
 *
 * Called by the Game Server SDK (or customer code if SDK is not used) to indicate that it is unready to receive allocations.
*/
class ROCKETSCIENCEMULTIPLAYSDK_API UnreadyServerRequest : public Request
{
public:
    virtual ~UnreadyServerRequest() {}
	void SetupHttpRequest(const FHttpRequestRef& HttpRequest) const final;
	FString ComputePath() const final;

	/* ID of the game server */
	int64 ServerId = 0;
};

class ROCKETSCIENCEMULTIPLAYSDK_API UnreadyServerResponse : public Response
{
public:
    virtual ~UnreadyServerResponse() {}
	void SetHttpResponseCode(EHttpResponseCodes::Type InHttpResponseCode) final;
	bool FromJson(const TSharedPtr<FJsonValue>& JsonValue) final;

    TSharedPtr<FJsonObject> Content;
};

}
