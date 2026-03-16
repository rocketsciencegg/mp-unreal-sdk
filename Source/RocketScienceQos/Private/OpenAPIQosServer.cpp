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

#include "OpenAPIQosServer.h"

#include "RSQosModule.h"
#include "OpenAPIHelpers.h"

#include "Templates/SharedPointer.h"

namespace RocketScienceQos
{

void OpenAPIQosServer::WriteJson(JsonWriter& Writer) const
{
	Writer->WriteObjectStart();
	Writer->WriteIdentifierPrefix(TEXT("locationid")); WriteJsonValue(Writer, Locationid);
	Writer->WriteIdentifierPrefix(TEXT("regionid")); WriteJsonValue(Writer, Regionid);
	Writer->WriteIdentifierPrefix(TEXT("ipv4")); WriteJsonValue(Writer, Ipv4);
	Writer->WriteIdentifierPrefix(TEXT("ipv6")); WriteJsonValue(Writer, Ipv6);
	Writer->WriteIdentifierPrefix(TEXT("port")); WriteJsonValue(Writer, Port);
	Writer->WriteObjectEnd();
}

bool OpenAPIQosServer::FromJson(const TSharedPtr<FJsonValue>& JsonValue)
{
	const TSharedPtr<FJsonObject>* Object;
	if (!JsonValue->TryGetObject(Object))
		return false;

	bool ParseSuccess = true;

	ParseSuccess &= TryGetJsonValue(*Object, TEXT("locationid"), Locationid);
	ParseSuccess &= TryGetJsonValue(*Object, TEXT("regionid"), Regionid);
	ParseSuccess &= TryGetJsonValue(*Object, TEXT("ipv4"), Ipv4);
	ParseSuccess &= TryGetJsonValue(*Object, TEXT("ipv6"), Ipv6);
	ParseSuccess &= TryGetJsonValue(*Object, TEXT("port"), Port);

	return ParseSuccess;
}

}
