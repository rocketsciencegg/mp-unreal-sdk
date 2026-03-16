 /*
 * #####################################################################################
 *  Unity Gaming Services Plugin for Unreal Engine Copyright © 2025 Unity Technologies.
 * #####################################################################################
 */

#include "Tests/AutomationCommon.h"
#include "QosApi.h"
#include "Utils/AutomationTestUtils.h"
#include "Utils/ServicesUtils.h"
#include "Utils/ServicesTestUtils.h"
#include "Mock/QosApi_AlwaysSucceeds.mock.h"

#if WITH_AUTOMATION_TESTS

#if ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION >= 5
BEGIN_DEFINE_SPEC(FRSQosApiSpec_SuccessTests, "QosSDK.FQosApi.SuccessTests", EAutomationTestFlags::ProductFilter | EAutomationTestFlags_ApplicationContextMask)
#else
BEGIN_DEFINE_SPEC(FQosApiSpec_SuccessTests, "QosSDK.FQosApi.SuccessTests", EAutomationTestFlags::ProductFilter | EAutomationTestFlags::ApplicationContextMask)
#endif
TUniquePtr<RocketScienceQos::FQosApi> Api;
END_DEFINE_SPEC(FRSQosApiSpec_SuccessTests)

void FRSQosApiSpec_SuccessTests::Define()
{

	Describe("When provided with valid arguments...", [this]()
		{
			BeforeEach([this]()
				{
					Api = MakeUnique<RocketScienceQos::FQosApi>(MakeUnique<RocketScienceQos::FQosApi_AlwaysSucceeds>());
				});

			/************************************************************************/
			/*           FQosApi::GetSortedQosResultsAsync() Tests              */
			/************************************************************************/

			LatentIt("GetSortedQosResultsAsync ListServers Response::bWasSuccessful should be true if the request was a success", [this](const FDoneDelegate& Done)
				{

					Api->GetSortedQosResultsAsync("", {}, Unity::Services::Core::THandler<FRSGetSortedQosResultsResponse>::CreateLambda([this, Done](FRSGetSortedQosResultsResponse Response)
						{
							/* Test success response */
							TestTrueExpr(Response.bWasSuccessful);
							Done.ExecuteIfBound();
						}));
				});
		});

	Describe("When calling GetSortedQosResultsAsync a second time before the first has finished...", [this]()
		{
			BeforeEach([this]()
				{
					Api = MakeUnique<RocketScienceQos::FQosApi>(MakeUnique<RocketScienceQos::FQosApi_AlwaysSucceeds>());
				});

			/************************************************************************/
			/*           FQosApi::GetSortedQosResultsAsync() Tests              */
			/************************************************************************/

			LatentIt("GetSortedQosResultsAsync GetServers Response::bWasSuccessful should be false if the request was a failure", [this](const FDoneDelegate& Done)
				{
					/*Account for error response */
					AddExpectedError(TEXT("Failed to begin process of retrieving sorted qos results as qos results are already in the process of being retrieved!"), EAutomationExpectedErrorFlags::Contains);

					Api->GetSortedQosResultsAsync("", {}, Unity::Services::Core::THandler<FRSGetSortedQosResultsResponse>::CreateLambda([this, Done](FRSGetSortedQosResultsResponse Response)
						{
							/* Test failure response */
							TestTrueExpr(Response.bWasSuccessful);
							Done.ExecuteIfBound();
						}));

					Api->GetSortedQosResultsAsync("", {}, nullptr);
				});
		});
}

#endif //WITH_AUTOMATION_TESTS
