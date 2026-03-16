 /*
 * #####################################################################################
 *  Unity Gaming Services Plugin for Unreal Engine Copyright © 2025 Unity Technologies.
 * #####################################################################################
 */

#pragma once
#include "IQosApi.h"
#include "Utils/ServicesTestUtils.h"
#include "OpenAPITicketsApiOperations.h"

#if WITH_AUTOMATION_TESTS

namespace RocketScienceQos 
{
	class FQosApi_DoesNothing : public IQosApi
		{
		public:
			~FQosApi_DoesNothing() override {}

			FHttpRequestPtr ListServers(const ListServersRequest& Request, const FListServersDelegate& Delegate = FListServersDelegate())
			{
				return FHttpRequestPtr();
			}
		};
}

#endif // WITH_AUTOMATION_TESTS