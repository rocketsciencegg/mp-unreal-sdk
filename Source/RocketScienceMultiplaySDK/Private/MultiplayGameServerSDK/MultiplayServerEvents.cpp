/*
 * #####################################################################################
 *  Rocket Science Multiplay Plugin for Unreal Engine Copyright © 2026 Rocket Science Group
 * #####################################################################################
 */

#include "MultiplayServerEvents.h"

namespace RocketScienceMultiplaySDK
{
	static constexpr const TCHAR* const kEventIdField = TEXT("EventID");
	static constexpr const TCHAR* const kEventTypeField = TEXT("EventType");
	static constexpr const TCHAR* const kServerIdField = TEXT("ServerID");
	static constexpr const TCHAR* const kAllocationIdField = TEXT("AllocationID");

	bool FMultiplayServerAllocateEvent::FromJson(const TSharedPtr<FJsonValue>& JsonValue)
	{
		const TSharedPtr<FJsonObject>* Object;
		if (!JsonValue->TryGetObject(Object))
			return false;

		bool bParseSuccess = true;

		bParseSuccess &= TryGetJsonValue(*Object, kEventIdField, EventId);
		bParseSuccess &= TryGetJsonValue(*Object, kEventTypeField, EventType);
		bParseSuccess &= EventType.Equals(kEventType);
		bParseSuccess &= TryGetJsonValue(*Object, kServerIdField, ServerId);
		bParseSuccess &= TryGetJsonValue(*Object, kAllocationIdField, AllocationId);

		return bParseSuccess;
	}

	bool FMultiplayServerDeallocateEvent::FromJson(const TSharedPtr<FJsonValue>& JsonValue)
	{
		const TSharedPtr<FJsonObject>* Object;
		if (!JsonValue->TryGetObject(Object))
			return false;

		bool bParseSuccess = true;

		bParseSuccess &= TryGetJsonValue(*Object, kEventIdField, EventId);
		bParseSuccess &= TryGetJsonValue(*Object, kEventTypeField, EventType);
		bParseSuccess &= EventType.Equals(kEventType);
		bParseSuccess &= TryGetJsonValue(*Object, kServerIdField, ServerId);
		bParseSuccess &= TryGetJsonValue(*Object, kAllocationIdField, AllocationId);

		return bParseSuccess;
	}
} // namespace RocketScienceMultiplaySDK