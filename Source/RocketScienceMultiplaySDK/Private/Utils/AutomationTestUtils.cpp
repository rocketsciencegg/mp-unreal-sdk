/*
 * #####################################################################################
 *  Rocket Science Multiplay Plugin for Unreal Engine Copyright © 2026 Rocket Science Group
 * #####################################################################################
 */

#include "AutomationTestUtils.h"

#ifdef WITH_AUTOMATION_TESTS

#include "Engine/Engine.h"
#include "Engine/World.h"

namespace RocketScienceMultiplaySDK
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
} // namespace RocketScienceMultiplaySDK

#endif // #ifdef WITH_AUTOMATION_TESTS
