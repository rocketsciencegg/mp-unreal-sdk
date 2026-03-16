/*
 * #####################################################################################
 *  Rocket Science Multiplay Plugin for Unreal Engine Copyright © 2026 Rocket Science Group
 * #####################################################################################
 */

#pragma once

#include "CoreMinimal.h"
#include "Utils/MultiplayJsonHelpers.h"

namespace RocketScienceMultiplaySDK
{
	class FMultiplayServerJson : IJsonReadable
	{
	public:
		virtual ~FMultiplayServerJson();
		virtual bool FromJson(const TSharedPtr<FJsonValue>& JsonValue) override;

	public:
		int64 ServerId;
		FString AllocationId;
		uint16 QueryPort;
		FString Ip;
		uint16 Port;
		FString ServerLogDirectory;
	};
} // namespace RocketScienceMultiplaySDK