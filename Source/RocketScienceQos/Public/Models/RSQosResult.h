 /*
 * #####################################################################################
 *  Rocket Science Multiplay Plugin for Unreal Engine Copyright © 2026 Rocket Science Group.
 * #####################################################################################
 */

#pragma once

#include "RSQosResult.generated.h"

USTRUCT(BlueprintType)
struct FRSQosResult 
{
	GENERATED_BODY()

	int NumResultsForRegion = 0;
	int32 AggregateLatency = 0;
	int32 AggregatePacketsLost = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rocket Science | Qos")
	int32 Latency = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rocket Science | Qos")
	int32 PacketLoss = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rocket Science | Qos")
	float PacketLossPercentage = 0.f; // Value between 0.0 and 1.0

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rocket Science | Qos")
	FString RegionId;
};