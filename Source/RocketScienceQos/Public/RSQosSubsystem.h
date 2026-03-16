 /*
 * #####################################################################################
 *  Rocket Science Multiplay Plugin for Unreal Engine Copyright © 2026 Rocket Science Group
 * #####################################################################################
 */

#pragma once

#include "CoreMinimal.h"
#include "Logging/LogMacros.h"
#include "Models/RSGetSortedQosResultsResponse.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Utils/PimplPtr.h"
#include "Utils/ServicesUtils.h"

#include "RSQosSubsystem.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogRSQosSubsystem, Warning, All);

namespace RocketScienceQos
{
	class FQosApi;
}

UCLASS()
class ROCKETSCIENCEQOS_API URSQosSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	/**
	* @brief Gets sorted QoS measurements for the specified fleet and regions.
	* @param FleetId			The fleet ID to query for QoS.
	* @param Regions			The regions to query for QoS.
	* @param ResponseHandler	The response callback to use once CreateTicket has finished.
	*/
	void GetSortedQosResultsAsync(FString FleetId, TArray<FString> Regions, Unity::Services::Core::THandler<FRSGetSortedQosResultsResponse> ResponseHandler);

private:
	/** Pointer to API implementation. */
	Unity::Services::Core::TPimplPtr<RocketScienceQos::FQosApi> ClientApi;
};