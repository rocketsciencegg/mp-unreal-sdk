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
#include "OpenAPIQosServer.h"

namespace RocketScienceQos
{

/*
 * OpenAPIQosServers
 *
 * List of available QoS Servers.
 */
class ROCKETSCIENCEQOS_API OpenAPIQosServers : public Model
{
public:
    virtual ~OpenAPIQosServers() {}
	bool FromJson(const TSharedPtr<FJsonValue>& JsonValue) final;
	void WriteJson(JsonWriter& Writer) const final;

	TOptional<TArray<OpenAPIQosServer>> Servers;
};

}
