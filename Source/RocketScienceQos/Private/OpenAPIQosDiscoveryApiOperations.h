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

#include "OpenAPIBaseModel.h"
#include "OpenAPIQosDiscoveryApi.h"

#include "OpenAPIQosServers.h"

namespace RocketScienceQos
{

/* List all QoS servers in fleet-regions associated with this fleet

*/
class ROCKETSCIENCEQOS_API ListServersRequest : public Request
{
public:
    virtual ~ListServersRequest() {}
	void SetupHttpRequest(const FHttpRequestRef& HttpRequest) const final;
	FString ComputePath() const final;

	/* Fleet ID */
	FString Fleetid;
	/* ETag value from a previous response for conditional request */
	TOptional<FString> IfNoneMatch;
};

class ROCKETSCIENCEQOS_API ListServersResponse : public Response
{
public:
    virtual ~ListServersResponse() {}
	void SetHttpResponseCode(EHttpResponseCodes::Type InHttpResponseCode) final;
	bool FromJson(const TSharedPtr<FJsonValue>& JsonValue) final;

    OpenAPIQosServers Content;
};

}
