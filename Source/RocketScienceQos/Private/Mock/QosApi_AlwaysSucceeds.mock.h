 /*
 * #####################################################################################
 *  Unity Gaming Services Plugin for Unreal Engine Copyright © 2025 Unity Technologies.
 * #####################################################################################
 */

#pragma once
#include "IQosApi.h"
#include "Models/RSGetSortedQosResultsResponse.h"
#include "Utils/ServicesTestUtils.h"

#if WITH_AUTOMATION_TESTS

namespace RocketScienceQos 
{
	class FQosApi_AlwaysSucceeds : public IQosApi
		{
		public:
			~FQosApi_AlwaysSucceeds() override {}

			FHttpRequestPtr ListServers(const ListServersRequest& Request, const FListServersDelegate& Delegate = FListServersDelegate())
			{
				ListServersResponse Response;
				Response.SetSuccessful(true);
				Response.SetHttpResponseCode(EHttpResponseCodes::Created);
				Response.SetResponseString(TEXT("Success"));
				OpenAPIQosServer TestServer;
				TestServer.Regionid = FGuid::NewGuid();
				TestServer.Ipv4 = "34.82.89.0:9000"; // This ip was pulled from existing unity mulitplay QoS servers, if success tests are failing when pinging, this ip may need to be updated
				Response.Content.Servers = { TestServer };
				Delegate.ExecuteIfBound(Response);

				return FHttpRequestPtr();
			}
		};
}

#endif // WITH_AUTOMATION_TESTS