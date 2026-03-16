/*
 * #####################################################################################
 *  Multiplay by Rocket Science Plugin for Unreal Engine Copyright © 2026 Rocket Science Corporation (UK) Ltd.
 * #####################################################################################
 */

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleInterface.h"
#include "Modules/ModuleManager.h"
#include "Logging/LogMacros.h"

 // Update Log Category Name
DECLARE_LOG_CATEGORY_EXTERN(LogRSQos, Warning, All);

// CRITICAL CHANGE: The API macro must match the new module name (ROCKETSCIENCEQOS_API)
class ROCKETSCIENCEQOS_API FRSQosModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};