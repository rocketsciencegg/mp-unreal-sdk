 /*
 * #####################################################################################
 *  Rocket Science Multiplay Plugin for Unreal Engine Copyright © 2026 Rocket Science Group
 * #####################################################################################
 */

#pragma once

//#include "ServicesCoreModule.h"

// Copied from Engine\Source\Runtime\Core\Public\Misc\AutomationTest.h.
// This #define doesn't exist in UE 4.22-4.24.
#ifndef WITH_AUTOMATION_TESTS
#define WITH_AUTOMATION_TESTS (WITH_DEV_AUTOMATION_TESTS || WITH_PERF_AUTOMATION_TESTS)
#endif

#ifndef TestFalseExpr
#define TestFalseExpr(Expression) TestFalse(TEXT(#Expression), Expression)
#endif