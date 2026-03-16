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

#include "OpenAPIPayloadAllocationErrorResponseBody.h"

#include "RocketScienceMultiplaySDKModule.h"
#include "OpenAPIHelpers.h"

#include "Templates/SharedPointer.h"

namespace RocketScienceMultiplaySDK
{

void OpenAPIPayloadAllocationErrorResponseBody::WriteJson(JsonWriter& Writer) const
{
	Writer->WriteObjectStart();
	Writer->WriteIdentifierPrefix(TEXT("success")); WriteJsonValue(Writer, Success);
	Writer->WriteIdentifierPrefix(TEXT("error")); WriteJsonValue(Writer, Error);
	Writer->WriteIdentifierPrefix(TEXT("error_code")); WriteJsonValue(Writer, ErrorCode);
	Writer->WriteIdentifierPrefix(TEXT("error_message")); WriteJsonValue(Writer, ErrorMessage);
	Writer->WriteObjectEnd();
}

bool OpenAPIPayloadAllocationErrorResponseBody::FromJson(const TSharedPtr<FJsonValue>& JsonValue)
{
	const TSharedPtr<FJsonObject>* Object;
	if (!JsonValue->TryGetObject(Object))
		return false;

	bool ParseSuccess = true;

	ParseSuccess &= TryGetJsonValue(*Object, TEXT("success"), Success);
	ParseSuccess &= TryGetJsonValue(*Object, TEXT("error"), Error);
	ParseSuccess &= TryGetJsonValue(*Object, TEXT("error_code"), ErrorCode);
	ParseSuccess &= TryGetJsonValue(*Object, TEXT("error_message"), ErrorMessage);

	return ParseSuccess;
}

}
