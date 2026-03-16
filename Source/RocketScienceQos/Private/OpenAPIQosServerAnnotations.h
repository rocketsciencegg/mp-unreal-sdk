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

namespace RocketScienceQos
{

/*
 * OpenAPIQosServerAnnotations
 *
 * A dictionary of server annotations.
 */
class ROCKETSCIENCEQOS_API OpenAPIQosServerAnnotations : public Model
{
public:
    virtual ~OpenAPIQosServerAnnotations() {}
	bool FromJson(const TSharedPtr<FJsonValue>& JsonValue) final;
	void WriteJson(JsonWriter& Writer) const final;

	TOptional<TArray<FString>> ProjectId;
	TOptional<TArray<FString>> EnvironmentId;
	/* Relay region in which this server is located. */
	TOptional<TArray<FString>> RelayRegionId;
	/* Multiplay region in which this server is located. */
	TOptional<TArray<FString>> MultiplayRegionId;
	/* List of Multiplay fleets for the current project & environment that have servers in the same region as this QoS server. Fleets that are not in ONLINE status are excluded. */
	TOptional<TArray<FString>> MultiplayFleetId;
	/* List of Matchmaker queues for the current project & environment that use fleets which have servers in the same region as this QoS server. Disabled queues are excluded. */
	TOptional<TArray<FString>> MatchmakerQueueName;
	/* List of Matchmaker pools for the current project & environment that use fleets which have servers in the same region as this QoS server. Disabled pools and pools that are part of disabled queues are excluded. */
	TOptional<TArray<FString>> MatchmakerPoolId;
};

}
