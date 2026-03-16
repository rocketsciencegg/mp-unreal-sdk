/*
 * #####################################################################################
 *  Rocket Science Multiplay Plugin for Unreal Engine Copyright © 2026 Rocket Science Group
 * #####################################################################################
 */

#pragma once

#include "CoreMinimal.h"
#include "RSMultiplayErrorResponse.generated.h"

USTRUCT(BlueprintType)
struct ROCKETSCIENCEMULTIPLAYSDK_API FRSMultiplayErrorResponse
{
	GENERATED_BODY()

	/* MUST use the same status code in the actual HTTP response. */
	UPROPERTY(BlueprintReadOnly, Category="Multiplay | Errors")
	int32 Status = 0;

	/* A human-readable explanation specific to this occurrence of the problem. Ought to focus on helping the client correct the problem, rather than giving debugging information. */
	UPROPERTY(BlueprintReadOnly, Category="Multiplay | Errors")
	FString Detail;

	/* SHOULD be the same as the recommended HTTP status phrase for that code. */
	UPROPERTY(BlueprintReadOnly, Category="Multiplay | Errors")
	FString Title;
};
