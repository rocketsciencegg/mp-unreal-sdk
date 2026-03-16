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
 * OpenAPIPayloadAllocationErrorResponseBody
 *
 * 
 */
class ROCKETSCIENCEMULTIPLAYSDK_API OpenAPIPayloadAllocationErrorResponseBody : public Model
{
public:
    virtual ~OpenAPIPayloadAllocationErrorResponseBody() {}
	bool FromJson(const TSharedPtr<FJsonValue>& JsonValue) final;
	void WriteJson(JsonWriter& Writer) const final;

	/* the call went through successfully */
	bool Success = false;
	/* there exists an error */
	bool Error = false;
	/* code of the error */
	int32 ErrorCode = 0;
	/* a message describing the error */
	FString ErrorMessage;
};

}
