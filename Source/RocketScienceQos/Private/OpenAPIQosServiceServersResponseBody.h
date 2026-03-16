/**
 * QoS Discovery
 * # Overview The QoS Discovery API enables clients to query the connection info of QoS servers in their regions of interest. Clients can use these servers to measure each region's quality of service (QoS) and make informed decisions to provide the best experience.  All responses are in JSON format and follow the [JSON:API specification](https://jsonapi.org).
 *
 * #####################################################################################
 *  Unity Gaming Services Plugin for Unreal Engine Copyright © 2025 Unity Technologies.
 * #####################################################################################
 * 
 * NOTE: This is a generated source file. Do not modify manually.
 */

#pragma once

#include "OpenAPIBaseModel.h"
#include "OpenAPIQosServiceServersList.h"

namespace RocketScienceQos
{

/*
 * OpenAPIQosServiceServersResponseBody
 *
 * 
 */
class ROCKETSCIENCEQOS_API OpenAPIQosServiceServersResponseBody : public Model
{
public:
    virtual ~OpenAPIQosServiceServersResponseBody() {}
	bool FromJson(const TSharedPtr<FJsonValue>& JsonValue) final;
	void WriteJson(JsonWriter& Writer) const final;

	OpenAPIQosServiceServersList Data;
};

}
