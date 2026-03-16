 /*
 * #####################################################################################
 *  Unity Gaming Services Plugin for Unreal Engine Copyright © 2025 Unity Technologies.
 * #####################################################################################
 */

#include "RSQosBlueprintApi.h"
#include "RSQosSubsystem.h"
#include "RSQosModule.h"
#include "Utils/ServicesUtils.h"


void URSQosBlueprintApi::GetSortedQosResultsAsync(FString FleetId, TArray<FString> Regions, FRSGetSortedQosResultsResponseDelegate ResponseHandler, const UObject* WorldContextObject)
{
	URSQosSubsystem* QosSubsystem = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull)->GetGameInstance()->GetSubsystem<URSQosSubsystem>();

	QosSubsystem->GetSortedQosResultsAsync(FleetId, Regions, Unity::Services::Core::THandler<FRSGetSortedQosResultsResponse>::CreateLambda([ResponseHandler](FRSGetSortedQosResultsResponse Response)
		{
			if (Response.bWasSuccessful)
			{
				UE_LOG(LogRSQos, Log, TEXT("Successfully retrieved sorted qos results"));
			}
			else
			{
				UE_LOG(LogRSQos, Error, TEXT("Failed to retrieve sorted qos results"));
			}

			ResponseHandler.ExecuteIfBound(Response);
		}));
}
