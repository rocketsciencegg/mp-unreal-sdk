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
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"
#include "Serialization/JsonSerializerMacros.h"

namespace RocketScienceQos
{

/*
 * OpenAPIQosServiceServer
 *
 * The connection information of a QoS server.
 */
class ROCKETSCIENCEQOS_API OpenAPIQosServiceServer : public Model
{
public:
    virtual ~OpenAPIQosServiceServer() {}
	bool FromJson(const TSharedPtr<FJsonValue>& JsonValue) final;
	void WriteJson(JsonWriter& Writer) const final;

	/* Endpoints at which you can reach the QoS server. */
	TArray<FString> Endpoints;
	/* The region to which the QoS server belongs. */
	FString Region;
	/* A dictionary of server annotations. */
	TOptional<TMap<FString, FJsonSerializableArray>> Annotations;
};

}
