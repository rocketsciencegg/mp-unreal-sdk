/*
 * #####################################################################################
 *  Rocket Science Multiplay Plugin for Unreal Engine Copyright © 2026 Rocket Science Group
 * #####################################################################################
 */

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "MultiplayGameServerSDKLog.h"

class FRocketScienceMultiplaySDKModule : public IModuleInterface
{
public:
	// IModuleInterface interface
	void StartupModule();
	void ShutdownModule();
};