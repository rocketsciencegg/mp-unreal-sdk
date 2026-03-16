/*
 * #####################################################################################
 *  Rocket Science Multiplay Plugin for Unreal Engine Copyright © 2026 Rocket Science Group
 * #####################################################################################
 */

#pragma once

#include "CoreMinimal.h"
#include "RSMultiplayDeallocation.generated.h"

/**
 * The Multiplay deallocation for the server.
 */
USTRUCT(BlueprintType)
struct ROCKETSCIENCEMULTIPLAYSDK_API FRSMultiplayDeallocation
{
    GENERATED_BODY()

     /**
     * The event ID for the deallocation.
     */
    UPROPERTY(BlueprintReadOnly, Category="Multiplay | Deallocation")
    FString EventId;

    /**
     * The server ID for the deallocation.
     */
    UPROPERTY(BlueprintReadOnly, Category="Multiplay | Deallocation")
    int64 ServerId = 0;

    /**
     * The ID for the deallocation.
     */
    UPROPERTY(BlueprintReadOnly, Category="Multiplay | Deallocation")
    FString AllocationId;
};
