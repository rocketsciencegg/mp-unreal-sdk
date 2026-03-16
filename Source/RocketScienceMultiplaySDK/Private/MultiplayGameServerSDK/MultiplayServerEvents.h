/*
 * #####################################################################################
 *  Rocket Science Multiplay Plugin for Unreal Engine Copyright © 2026 Rocket Science Group
 * #####################################################################################
 */

#pragma once

#include "Utils/MultiplayJsonHelpers.h"

namespace RocketScienceMultiplaySDK
{
	// {
	//   "EventID": "e3e455f8-f977-11e9-bccf-1a111111f111",
	//   "EventType": "AllocateEventType",
	//   "ServerID": 1,
	//   "AllocationID": "e3e455f8-f977-11e9-bccf-2a222222f222"
	// }
	class FMultiplayServerAllocateEvent : public IJsonReadable
	{
	public:
		static constexpr const TCHAR* const kEventType = TEXT("AllocateEventType");

		virtual ~FMultiplayServerAllocateEvent() = default;

		virtual bool FromJson(const TSharedPtr<FJsonValue>& JsonValue) override;

	public:
		FGuid EventId;
		FString EventType;
		int64 ServerId;
		FGuid AllocationId;
	};

	// {
	//   "EventID": "e3e455f8-f977-11e9-bccf-1a111111f111",
	//   "EventType": "DeallocateEventType",
	//   "ServerID": 1,
	//   "AllocationID": "e3e455f8-f977-11e9-bccf-2a222222f222"
	// }
	class FMultiplayServerDeallocateEvent : public IJsonReadable
	{
	public:
		static constexpr const TCHAR* const kEventType = TEXT("DeallocateEventType");

		virtual ~FMultiplayServerDeallocateEvent() = default;

		virtual bool FromJson(const TSharedPtr<FJsonValue>& JsonValue) override;

	public:
		FGuid EventId;
		FString EventType;
		int64 ServerId;
		FGuid AllocationId;
	};
} // namespace RocketScienceMultiplaySDK