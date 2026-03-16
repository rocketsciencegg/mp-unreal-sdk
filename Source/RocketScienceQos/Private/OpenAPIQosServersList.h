/**
 * QoS Discovery
 * # Overview The QoS Discovery API enables clients to query the connection info of QoS servers in their regions of interest. Clients can use these servers to measure each region's quality of service (QoS) and make informed decisions to provide the best experience.
 *
 * #####################################################################################
 *  Unity Gaming Services Plugin for Unreal Engine Copyright © 2025 Unity Technologies.
 * #####################################################################################
 * 
 * NOTE: This is a generated source file. Do not modify manually.
 */

#pragma once

#include "OpenAPIBaseModel.h"
#include "OpenAPIQosServer.h"

namespace RocketScienceQos
{

/*
 * OpenAPIQosServersList
 *
 * 
 */
class ROCKETSCIENCEQOS_API OpenAPIQosServersList : public Model
{
public:
    virtual ~OpenAPIQosServersList() {}
	bool FromJson(const TSharedPtr<FJsonValue>& JsonValue) final;
	void WriteJson(JsonWriter& Writer) const final;

	/* An array of connection information for QoS servers. */
	TArray<OpenAPIQosServer> Servers;
};

}
