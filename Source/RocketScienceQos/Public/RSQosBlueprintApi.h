 /*
 * #####################################################################################
 *  Unity Gaming Services Plugin for Unreal Engine Copyright © 2025 Unity Technologies.
 * #####################################################################################
 */

#pragma once

#include "Engine/Engine.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "Models/RSGetSortedQosResultsResponse.h"
#include "RSQosBlueprintApi.generated.h"

UCLASS()
class URSQosBlueprintApi : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:

	DECLARE_DYNAMIC_DELEGATE_OneParam(FRSGetSortedQosResultsResponseDelegate, FRSGetSortedQosResultsResponse, Response);

	/**
	* @brief Gets sorted QoS measurements for the specified fleet and regions.
	* @param FleetId			The fleet ID to query for QoS.
	* @param Regions			The regions to query for QoS.
	* @param ResponseHandler	The response callback to use once CreateTicket has finished.
	*/
	UFUNCTION(BlueprintCallable, Category = "Rocket Science | Qos", meta = (WorldContext = "WorldContextObject"))
	static void GetSortedQosResultsAsync(FString FleetId, TArray<FString> Regions, FRSGetSortedQosResultsResponseDelegate ResponseHandler, const UObject* WorldContextObject);
};