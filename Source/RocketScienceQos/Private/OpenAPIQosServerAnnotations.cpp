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

#include "OpenAPIQosServerAnnotations.h"

#include "RSQosModule.h"
#include "OpenAPIHelpers.h"

#include "Templates/SharedPointer.h"

namespace RocketScienceQos
{

void OpenAPIQosServerAnnotations::WriteJson(JsonWriter& Writer) const
{
	Writer->WriteObjectStart();
	if (ProjectId.IsSet())
	{
		Writer->WriteIdentifierPrefix(TEXT("projectId")); WriteJsonValue(Writer, ProjectId.GetValue());
	}
	if (EnvironmentId.IsSet())
	{
		Writer->WriteIdentifierPrefix(TEXT("environmentId")); WriteJsonValue(Writer, EnvironmentId.GetValue());
	}
	if (RelayRegionId.IsSet())
	{
		Writer->WriteIdentifierPrefix(TEXT("relayRegionId")); WriteJsonValue(Writer, RelayRegionId.GetValue());
	}
	if (MultiplayRegionId.IsSet())
	{
		Writer->WriteIdentifierPrefix(TEXT("multiplayRegionId")); WriteJsonValue(Writer, MultiplayRegionId.GetValue());
	}
	if (MultiplayFleetId.IsSet())
	{
		Writer->WriteIdentifierPrefix(TEXT("multiplayFleetId")); WriteJsonValue(Writer, MultiplayFleetId.GetValue());
	}
	if (MatchmakerQueueName.IsSet())
	{
		Writer->WriteIdentifierPrefix(TEXT("matchmakerQueueName")); WriteJsonValue(Writer, MatchmakerQueueName.GetValue());
	}
	if (MatchmakerPoolId.IsSet())
	{
		Writer->WriteIdentifierPrefix(TEXT("matchmakerPoolId")); WriteJsonValue(Writer, MatchmakerPoolId.GetValue());
	}
	Writer->WriteObjectEnd();
}

bool OpenAPIQosServerAnnotations::FromJson(const TSharedPtr<FJsonValue>& JsonValue)
{
	const TSharedPtr<FJsonObject>* Object;
	if (!JsonValue->TryGetObject(Object))
		return false;

	bool ParseSuccess = true;

	TryGetJsonValue(*Object, TEXT("projectId"), ProjectId);
	TryGetJsonValue(*Object, TEXT("environmentId"), EnvironmentId);
	TryGetJsonValue(*Object, TEXT("relayRegionId"), RelayRegionId);
	TryGetJsonValue(*Object, TEXT("multiplayRegionId"), MultiplayRegionId);
	TryGetJsonValue(*Object, TEXT("multiplayFleetId"), MultiplayFleetId);
	TryGetJsonValue(*Object, TEXT("matchmakerQueueName"), MatchmakerQueueName);
	TryGetJsonValue(*Object, TEXT("matchmakerPoolId"), MatchmakerPoolId);

	return ParseSuccess;
}

}
