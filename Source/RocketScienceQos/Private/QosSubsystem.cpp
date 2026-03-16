 /*
 * #####################################################################################
 *  Unity Gaming Services Plugin for Unreal Engine Copyright © 2025 Unity Technologies.
 * #####################################################################################
 */

#include "RSQosSubsystem.h"

#include "QosApi.h"
#include "WrappedQosApi.h"

DEFINE_LOG_CATEGORY(LogRSQosSubsystem);

void URSQosSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	// TODO (Jac Griffiths): Pass in our own auth
	// 
	// This subsystem is dependent on the the auth token from the Authentication subsystem
	//Collection.InitializeDependency(UAuthenticationSubsystem::StaticClass());

	ClientApi = Unity::Services::Core::MakePimpl<RocketScienceQos::FQosApi>(MakeUnique<RocketScienceQos::FWrappedQosApi>(/*GetGameInstance()->GetSubsystem<UAuthenticationSubsystem>()*/));
}

void URSQosSubsystem::GetSortedQosResultsAsync(FString FleetId, TArray<FString> Regions, Unity::Services::Core::THandler<FRSGetSortedQosResultsResponse> ResponseHandler)
{
	ClientApi->GetSortedQosResultsAsync(FleetId, Regions, ResponseHandler);
}