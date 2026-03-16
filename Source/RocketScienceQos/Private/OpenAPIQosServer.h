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

namespace RocketScienceQos
{

/*
 * OpenAPIQosServer
 *
 * Details of a given QoS Server
 */
class ROCKETSCIENCEQOS_API OpenAPIQosServer : public Model
{
public:
    virtual ~OpenAPIQosServer() {}
	bool FromJson(const TSharedPtr<FJsonValue>& JsonValue) final;
	void WriteJson(JsonWriter& Writer) const final;

	/* ID of the location that this QoS server is in */
	int64 Locationid = 0;
	/* Region ID of the qos server */
	FGuid Regionid;
	/* IPv4 address of QoS Server */
	FString Ipv4;
	/* IPv6 address of QoS Server */
	FString Ipv6;
	/* Port of the QoS Server */
	int32 Port = 0;
};

}
