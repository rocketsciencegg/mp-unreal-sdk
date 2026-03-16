 /*
 * #####################################################################################
 *  Unity Gaming Services Plugin for Unreal Engine Copyright © 2025 Unity Technologies.
 * #####################################################################################
 */

#pragma once
#include "IQosApi.h"
#include "OpenAPIQosDiscoveryApi.h"

namespace RocketScienceQos 
{	
	class FWrappedQosApi : public IQosApi
	{
	public:
		FWrappedQosApi();
		
		// TODO (Jac Griffiths): Re-enable when we have auth available
		//FWrappedQosApi(UAuthenticationSubsystem* AuthenticationSubsystem);

		virtual ~FWrappedQosApi() override = default;

		virtual FHttpRequestPtr ListServers(const ListServersRequest& Request, const FListServersDelegate& Delegate = FListServersDelegate()) override;

	private:
		TUniquePtr<OpenAPIQosDiscoveryApi> QosApi;

		//UAuthenticationSubsystem* AuthenticationSubsystem;
	};

}