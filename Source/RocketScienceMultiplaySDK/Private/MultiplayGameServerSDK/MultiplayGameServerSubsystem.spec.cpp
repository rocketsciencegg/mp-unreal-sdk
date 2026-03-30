/*
 * #####################################################################################
 *  Rocket Science Multiplay Plugin for Unreal Engine Copyright © 2026 Rocket Science Group
 * #####################################################################################
 */

#include "Tests/AutomationCommon.h"
#include "Utils/AutomationTestUtils.h"
#include "Engine/World.h"
#include "RSMultiplayGameServerSubsystem.h"

#if WITH_AUTOMATION_TESTS
BEGIN_DEFINE_SPEC(FRSMultiplayGameServerSubsystemSpec, "MultiplayGameServerSDK.URSMultiplayGameServerSubsystem", EAutomationTestFlags::ProductFilter | EAutomationTestFlags::ApplicationContextMask)
URSMultiplayGameServerSubsystem* GameServerSubsystem;
END_DEFINE_SPEC(FRSMultiplayGameServerSubsystemSpec)

void FRSMultiplayGameServerSubsystemSpec::Define()
{
	BeforeEach([this]()
		{
			AutomationOpenMap(TEXT("/UnityGamingServicesSDK/Test"));

			UWorld* GameWorld = RocketScienceMultiplaySDK::GetAnyGameWorld();
			UGameInstance* GameInstance = GameWorld->GetGameInstance();
			GameServerSubsystem = GameInstance->GetSubsystem<URSMultiplayGameServerSubsystem>();
		});

	// TODO: Declare test cases for URSMultiplayGameServerSubsystem.

	AfterEach([this]()
		{
			GameServerSubsystem = nullptr;
		});
}

#endif // #if WITH_AUTOMATION_TESTS
