 /*
 * #####################################################################################
 *  Unity Gaming Services Plugin for Unreal Engine Copyright © 2025 Unity Technologies.
 * #####################################################################################
 */

#pragma once

#include "RSQosServer.h"
#include "Models/RSQosResult.h"
#include "RSGetSortedQosResultsResponse.generated.h"

USTRUCT(BlueprintType)
struct FRSGetSortedQosResultsResponse
{
	GENERATED_BODY()

	/* Indicates if the request was successfully or not. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Unity Gaming Services | Qos | Model")
	bool bWasSuccessful = false;

	/* An error message detailing the error. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Unity Gaming Services | Qos | Model")
	FString ErrorMessage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Unity Gaming Services | Qos | Model")
	TArray<FRSQosResult> QosResults;
};