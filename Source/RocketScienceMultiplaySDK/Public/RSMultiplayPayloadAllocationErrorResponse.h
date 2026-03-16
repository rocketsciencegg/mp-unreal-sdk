/*
 * #####################################################################################
 *  Rocket Science Multiplay Plugin for Unreal Engine Copyright © 2026 Rocket Science Group
 * #####################################################################################
 */

#pragma once

#include "CoreMinimal.h"
#include "RSMultiplayPayloadAllocationErrorResponse.generated.h"

USTRUCT(BlueprintType)
struct ROCKETSCIENCEMULTIPLAYSDK_API FRSMultiplayPayloadAllocationErrorResponse
{
	GENERATED_BODY()

	/* the call went through successfully */
	UPROPERTY(BlueprintReadOnly, Category="Multiplay | Errors")
	bool Success = false;

	/* there exists an error */
	UPROPERTY(BlueprintReadOnly, Category="Multiplay | Errors")
	bool Error = false;

	/* code of the error */
	UPROPERTY(BlueprintReadOnly, Category="Multiplay | Errors")
	int32 ErrorCode = 0;

	/* a message describing the error */
	UPROPERTY(BlueprintReadOnly, Category="Multiplay | Errors")
	FString ErrorMessage;
};
