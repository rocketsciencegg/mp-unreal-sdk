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

#include "OpenAPIQosServers.h"

#include "RSQosModule.h"
#include "OpenAPIHelpers.h"

#include "Templates/SharedPointer.h"

namespace RocketScienceQos
{

void OpenAPIQosServers::WriteJson(JsonWriter& Writer) const
{
	Writer->WriteObjectStart();
	if (Servers.IsSet())
	{
		Writer->WriteIdentifierPrefix(TEXT("servers")); WriteJsonValue(Writer, Servers.GetValue());
	}
	Writer->WriteObjectEnd();
}

bool OpenAPIQosServers::FromJson(const TSharedPtr<FJsonValue>& JsonValue)
{
	const TSharedPtr<FJsonObject>* Object;
	if (!JsonValue->TryGetObject(Object))
		return false;

	bool ParseSuccess = true;

	TryGetJsonValue(*Object, TEXT("servers"), Servers);

	return ParseSuccess;
}

}
