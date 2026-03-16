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
 * OpenAPIPayloadTokenResponseBody
 *
 * 
 */
class ROCKETSCIENCEMULTIPLAYSDK_API OpenAPIPayloadTokenResponseBody : public Model
{
public:
    virtual ~OpenAPIPayloadTokenResponseBody() {}
	bool FromJson(const TSharedPtr<FJsonValue>& JsonValue) final;
	void WriteJson(JsonWriter& Writer) const final;

	/* JWT Token string associated to payload requests */
	FString Token;
	/* Internal multiplay error occurred retrieving the JWT */
	FString Error;
};

}
