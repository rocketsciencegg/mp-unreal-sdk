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

namespace RocketScienceMultiplaySDK
{

/*
 * OpenAPIErrorResponseBody
 *
 * 
 */
class ROCKETSCIENCEMULTIPLAYSDK_API OpenAPIErrorResponseBody : public Model
{
public:
    virtual ~OpenAPIErrorResponseBody() {}
	bool FromJson(const TSharedPtr<FJsonValue>& JsonValue) final;
	void WriteJson(JsonWriter& Writer) const final;

	/* MUST use the same status code in the actual HTTP response. */
	int32 Status = 0;
	/* A human-readable explanation specific to this occurrence of the problem. Ought to focus on helping the client correct the problem, rather than giving debugging information. */
	FString Detail;
	/* SHOULD be the same as the recommended HTTP status phrase for that code. */
	FString Title;
};

}
