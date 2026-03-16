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

#include "OpenAPIPayloadTokenResponseBody.h"

#include "RocketScienceMultiplaySDKModule.h"
#include "OpenAPIHelpers.h"

#include "Templates/SharedPointer.h"

namespace RocketScienceMultiplaySDK
{

void OpenAPIPayloadTokenResponseBody::WriteJson(JsonWriter& Writer) const
{
	Writer->WriteObjectStart();
	Writer->WriteIdentifierPrefix(TEXT("token")); WriteJsonValue(Writer, Token);
	Writer->WriteIdentifierPrefix(TEXT("error")); WriteJsonValue(Writer, Error);
	Writer->WriteObjectEnd();
}

bool OpenAPIPayloadTokenResponseBody::FromJson(const TSharedPtr<FJsonValue>& JsonValue)
{
	const TSharedPtr<FJsonObject>* Object;
	if (!JsonValue->TryGetObject(Object))
		return false;

	bool ParseSuccess = true;

	ParseSuccess &= TryGetJsonValue(*Object, TEXT("token"), Token);
	ParseSuccess &= TryGetJsonValue(*Object, TEXT("error"), Error);

	return ParseSuccess;
}

}
