 /*
 * #####################################################################################
 *  Unity Gaming Services Plugin for Unreal Engine Copyright © 2025 Unity Technologies.
 * #####################################################################################
 */

#pragma once
#include "IQosApi.h"
#include "Utils/ServicesTestUtils.h"
#include "RSQosModule.h"

#if WITH_AUTOMATION_TESTS

namespace RocketScienceQos 
{
	class FQosApi_AlwaysFails : public IQosApi
		{
		public:
			~FQosApi_AlwaysFails() override {}

			FHttpRequestPtr ListServers(const ListServersRequest& Request, const FListServersDelegate& Delegate = FListServersDelegate())
			{
				ListServersResponse Response;
				Response.SetSuccessful(false);
				Response.SetHttpResponseCode(EHttpResponseCodes::Denied);
				Response.SetResponseString(TEXT("You are not signed in to the Authentication Service. Please sign in."));
				Delegate.ExecuteIfBound(Response);

				return FHttpRequestPtr();
			}
		};
}

#endif // WITH_AUTOMATION_TESTS