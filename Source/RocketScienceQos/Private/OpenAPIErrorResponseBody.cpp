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

#include "OpenAPIErrorResponseBody.h"

#include "RSQosModule.h"
#include "OpenAPIHelpers.h"

#include "Templates/SharedPointer.h"

namespace RocketScienceQos
{

void OpenAPIErrorResponseBody::WriteJson(JsonWriter& Writer) const
{
	Writer->WriteObjectStart();
	Writer->WriteIdentifierPrefix(TEXT("type")); WriteJsonValue(Writer, Type);
	Writer->WriteIdentifierPrefix(TEXT("title")); WriteJsonValue(Writer, Title);
	Writer->WriteIdentifierPrefix(TEXT("status")); WriteJsonValue(Writer, Status);
	Writer->WriteIdentifierPrefix(TEXT("code")); WriteJsonValue(Writer, Code);
	Writer->WriteIdentifierPrefix(TEXT("detail")); WriteJsonValue(Writer, Detail);
	if (Instance.IsSet())
	{
		Writer->WriteIdentifierPrefix(TEXT("instance")); WriteJsonValue(Writer, Instance.GetValue());
	}
	if (Details.IsSet())
	{
		Writer->WriteIdentifierPrefix(TEXT("details")); WriteJsonValue(Writer, Details.GetValue());
	}
	Writer->WriteObjectEnd();
}

bool OpenAPIErrorResponseBody::FromJson(const TSharedPtr<FJsonValue>& JsonValue)
{
	const TSharedPtr<FJsonObject>* Object;
	if (!JsonValue->TryGetObject(Object))
		return false;

	bool ParseSuccess = true;

	ParseSuccess &= TryGetJsonValue(*Object, TEXT("type"), Type);
	ParseSuccess &= TryGetJsonValue(*Object, TEXT("title"), Title);
	ParseSuccess &= TryGetJsonValue(*Object, TEXT("status"), Status);
	ParseSuccess &= TryGetJsonValue(*Object, TEXT("code"), Code);
	ParseSuccess &= TryGetJsonValue(*Object, TEXT("detail"), Detail);
	TryGetJsonValue(*Object, TEXT("instance"), Instance);
	TryGetJsonValue(*Object, TEXT("details"), Details);

	return ParseSuccess;
}

}
