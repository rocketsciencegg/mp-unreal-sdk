 /*
 * #####################################################################################
 *  Unity Gaming Services Plugin for Unreal Engine Copyright © 2025 Unity Technologies.
 * #####################################################################################
 */

#include "AutomationTestUtils.h"
#include "Engine/Engine.h"

namespace RocketScienceQos
{
	// Copied from Engine/Source/Runtime/Engine/Private/Tests/AutomationCommon.cpp.
	// This was the only example I could find demonstrating how to obtain a UWorld reference inside of an automation test.
	UWorld* GetAnyGameWorld()
	{
		UWorld* TestWorld = nullptr;
		const TIndirectArray<FWorldContext>& WorldContexts = GEngine->GetWorldContexts();
		for (const FWorldContext& Context : WorldContexts)
		{
			if (((Context.WorldType == EWorldType::PIE) || (Context.WorldType == EWorldType::Game)) && (Context.World() != NULL))
			{
				TestWorld = Context.World();
				break;
			}
		}

		return TestWorld;
	}
} // namespace RocketScienceQos