 /*
 * #####################################################################################
 *  Unity Gaming Services Plugin for Unreal Engine Copyright © 2025 Unity Technologies.
 * #####################################################################################
 */

#pragma once

#include "CoreMinimal.h"

#define MOCK_FUNC_NOT_IMPLEMENTED(funcname) \
	UE_LOG(LogRSQos, Verbose, TEXT(funcname) TEXT(": Called but there is no implementation."))

namespace Authentication 
{
    UWorld* GetAnyGameWorld();
}