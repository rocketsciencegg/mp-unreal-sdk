/*
 * #####################################################################################
 *  Rocket Science Multiplay Plugin for Unreal Engine Copyright © 2026 Rocket Science Group
 * #####################################################################################
 */
 
#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "RSMultiplaySubsystemBase.generated.h"

/** 
  * @brief Base class for Multiplay module subsystems that define common definitions.
  */
UCLASS(Abstract, Within = GameInstance)
class ROCKETSCIENCEMULTIPLAYSDK_API URSMultiplaySubsystemBase : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:

	const FString LocalServerFlag = TEXT("localserver");

	/**
	 * Override ShouldCreate to prevent subsystem spinup in local servers.
	 */
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
};