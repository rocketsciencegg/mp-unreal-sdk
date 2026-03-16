/**
 * QoS Discovery
 * # Overview The QoS Discovery API enables clients to query the connection info of QoS servers in their regions of interest. Clients can use these servers to measure each region's quality of service (QoS) and make informed decisions to provide the best experience.
 *
 * #####################################################################################
 *  Multiplay by Rocket Science Plugin for Unreal Engine Copyright © 2026 Rocket Science Corporation (UK) Ltd.
 * #####################################################################################
 * 
 * NOTE: This is a generated source file. Do not modify manually.
 */

#include "OpenAPIQosServersResponseBody.h"

#include "RSQosModule.h"
#include "OpenAPIHelpers.h"

#include "Templates/SharedPointer.h"

namespace RocketScienceQos
{

void OpenAPIQosServersResponseBody::WriteJson(JsonWriter& Writer) const
{
	Writer->WriteObjectStart();
	Writer->WriteIdentifierPrefix(TEXT("data")); WriteJsonValue(Writer, Data);
	Writer->WriteObjectEnd();
}

bool OpenAPIQosServersResponseBody::FromJson(const TSharedPtr<FJsonValue>& JsonValue)
{
	const TSharedPtr<FJsonObject>* Object;
	if (!JsonValue->TryGetObject(Object))
		return false;

	bool ParseSuccess = true;

	ParseSuccess &= TryGetJsonValue(*Object, TEXT("data"), Data);

	return ParseSuccess;
}

}
