/*
 * #####################################################################################
 *  Rocket Science Multiplay Plugin for Unreal Engine Copyright © 2026 Rocket Science Group
 * #####################################################################################
 */

#pragma once

#include "CoreMinimal.h"
#include "RSMultiplayPayloadTokenResponse.generated.h"

USTRUCT(BlueprintType)
struct ROCKETSCIENCEMULTIPLAYSDK_API FRSMultiplayPayloadTokenResponse
{
	GENERATED_BODY()

	/* JWT Token string associated to payload requests */
	UPROPERTY(BlueprintReadOnly, Category="Multiplay | PayloadToken")
	FString Token;

	/* Internal multiplay error occurred retrieving the JWT */
	UPROPERTY(BlueprintReadOnly, Category="Multiplay | PayloadToken")
	FString Error;
};
