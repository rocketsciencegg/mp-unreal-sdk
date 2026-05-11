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
#include "Mock/QosApi_AlwaysFails.mock.h"

#if WITH_AUTOMATION_TESTS
BEGIN_DEFINE_SPEC(FRSQosApiSpec_FailureTests, "QosSDK.FQosApi.FailureTests",
	EAutomationTestFlags::ProductFilter | 
	EAutomationTestFlags::EditorContext | 
	EAutomationTestFlags::ClientContext | 
	EAutomationTestFlags::ServerContext | 
	EAutomationTestFlags::CommandletContext)
END_DEFINE_SPEC(FRSQosApiSpec_FailureTests)

void FRSQosApiSpec_FailureTests::Define()
{
	Describe("When getting servers fails...", [this]()
		{
			BeforeEach([this]()
				{
					Api = MakeUnique<RocketScienceQos::FQosApi>(MakeUnique<RocketScienceQos::FQosApi_AlwaysFails>());
				});

			/************************************************************************/
			/*           FQosApi::GetSortedQosResultsAsync() Tests              */
			/************************************************************************/

			LatentIt("GetSortedQosResultsAsync GetServers Response::bWasSuccessful should be false if the request was a failure", [this](const FDoneDelegate& Done)
				{
					/*Account for error response */
					AddExpectedError(TEXT("Invalid server response when trying to get servers!"), EAutomationExpectedErrorFlags::Contains);

					Api->GetSortedQosResultsAsync("", {}, Unity::Services::Core::THandler<FRSGetSortedQosResultsResponse>::CreateLambda([this, Done](FRSGetSortedQosResultsResponse Response)
						{
							/* Test failure response */
							TestFalseExpr(Response.bWasSuccessful);
							Done.ExecuteIfBound();
						}));
				});
		});
}

#endif //WITH_AUTOMATION_TESTS
