/**
 * QoS Discovery
 * # Overview The QoS Discovery API enables clients to query the connection info of QoS servers in their regions of interest. Clients can use these servers to measure each region's quality of service (QoS) and make informed decisions to provide the best experience.  All responses are in JSON format and follow the [JSON:API specification](https://jsonapi.org).
 *
 * #####################################################################################
 *  Unity Gaming Services Plugin for Unreal Engine Copyright © 2025 Unity Technologies.
 * #####################################################################################
 * 
 * NOTE: This is a generated source file. Do not modify manually.
 */

#include "OpenAPIQosServiceServer.h"

#include "RSQosModule.h"
#include "OpenAPIHelpers.h"

#include "Templates/SharedPointer.h"

namespace RocketScienceQos
{

void OpenAPIQosServiceServer::WriteJson(JsonWriter& Writer) const
{
	Writer->WriteObjectStart();
	Writer->WriteIdentifierPrefix(TEXT("endpoints")); WriteJsonValue(Writer, Endpoints);
	Writer->WriteIdentifierPrefix(TEXT("region")); WriteJsonValue(Writer, Region);
	if (Annotations.IsSet())
	{
		Writer->WriteIdentifierPrefix(TEXT("annotations")); WriteJsonValue(Writer, Annotations.GetValue());
	}
	Writer->WriteObjectEnd();
}

bool OpenAPIQosServiceServer::FromJson(const TSharedPtr<FJsonValue>& JsonValue)
{
	const TSharedPtr<FJsonObject>* Object;
	if (!JsonValue->TryGetObject(Object))
		return false;

	bool ParseSuccess = true;

	ParseSuccess &= TryGetJsonValue(*Object, TEXT("endpoints"), Endpoints);
	ParseSuccess &= TryGetJsonValue(*Object, TEXT("region"), Region);
	ParseSuccess &= TryGetJsonValue(*Object, TEXT("annotations"), Annotations);

	return ParseSuccess;
}

}
