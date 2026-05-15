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
BEGIN_DEFINE_SPEC(FRSQosApiSpec_OperationalTests, "QosSDK.FQosApi.OperationalTests",
	EAutomationTestFlags::ProductFilter | 
	EAutomationTestFlags::EditorContext | 
	EAutomationTestFlags::ClientContext | 
	EAutomationTestFlags::ServerContext | 
	EAutomationTestFlags::CommandletContext)
TUniquePtr<RocketScienceQos::FQosApi> Api;
END_DEFINE_SPEC(FRSQosApiSpec_OperationalTests)

void FRSQosApiSpec_OperationalTests::Define()
{

}

#endif //WITH_AUTOMATION_TESTS
