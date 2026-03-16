/*
 * #####################################################################################
 *  Rocket Science Multiplay Plugin for Unreal Engine Copyright © 2026 Rocket Science Group
 * #####################################################################################
 */

#pragma once

#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"

// Copied from Engine\Source\Runtime\Core\Public\Misc\AutomationTest.h.
// This #define doesn't exist in UE 4.22-4.24.
#ifndef WITH_AUTOMATION_TESTS
#define WITH_AUTOMATION_TESTS (WITH_DEV_AUTOMATION_TESTS || WITH_PERF_AUTOMATION_TESTS)
#endif

#ifdef WITH_AUTOMATION_TESTS

// This macro exists because there isn't an equivalent to TestTrueExpr() in AutomationTest.h
#define TestFalseExpr(Expression) TestFalse(TEXT(#Expression), Expression)

// This macro exists because prior to UE 4.25 TestTrue() returned void rather than bool.
#define MP_TEST_TRUE_EXPR(Expression) \
[&]() \
{ \
    bool bResult = Expression; \
    TestTrueExpr(bResult); \
    return bResult; \
}() \

// This macro exists because prior to UE 4.25 TestFalse() returned void rather than bool.
#define MP_TEST_FALSE_EXPR(Expression) \
[&]() \
{ \
    bool bResult = Expression; \
    TestFalseExpr(bResult); \
    return bResult; \
}() \

// This macro exists because prior to UE 4.25 TestValid() returned void rather than bool.
#define MP_TEST_VALID(Description, SharedPointer) \
[&]() \
{ \
bool bResult = SharedPointer.IsValid(); \
TestValid(Description, SharedPointer); \
return bResult; \
}() \

namespace RocketScienceMultiplaySDK
{
    UWorld* GetAnyGameWorld();
}

#endif // #ifdef WITH_AUTOMATION_TESTS