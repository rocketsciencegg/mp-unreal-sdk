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
BEGIN_DEFINE_SPEC(FRSQosApiSpec_OperationalTests, "QosSDK.FQosApi.OperationalTests", EAutomationTestFlags::ProductFilter | EAutomationTestFlags_ApplicationContextMask)
#else
BEGIN_DEFINE_SPEC(FQosApiSpec_OperationalTests, "QosSDK.FQosApi.OperationalTests", EAutomationTestFlags::ProductFilter | EAutomationTestFlags::ApplicationContextMask)
#endif
TUniquePtr<RocketScienceQos::FQosApi> Api;
END_DEFINE_SPEC(FRSQosApiSpec_OperationalTests)

void FRSQosApiSpec_OperationalTests::Define()
{

}

#endif //WITH_AUTOMATION_TESTS
