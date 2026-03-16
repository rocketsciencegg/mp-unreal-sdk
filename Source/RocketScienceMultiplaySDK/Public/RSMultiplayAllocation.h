/*
 * #####################################################################################
 *  Rocket Science Multiplay Plugin for Unreal Engine Copyright © 2026 Rocket Science Group
 * #####################################################################################
 */

#pragma once

#include "CoreMinimal.h"
#include "RSMultiplayAllocation.generated.h"

/**
 * The Multiplay allocation for the server.
 */
USTRUCT(BlueprintType)
struct ROCKETSCIENCEMULTIPLAYSDK_API FRSMultiplayAllocation
{
    GENERATED_BODY()

    /**
     * The event ID for the allocation.
     */
    UPROPERTY(BlueprintReadOnly, Category="Multiplay | Allocation")
    FString EventId;

    /**
     * The server ID for the allocation.
     */
    UPROPERTY(BlueprintReadOnly, Category="Multiplay | Allocation")
    int64 ServerId = 0;

    /**
     * The ID for the allocation.
     */
    UPROPERTY(BlueprintReadOnly, Category="Multiplay | Allocation")
    FString AllocationId;
};
