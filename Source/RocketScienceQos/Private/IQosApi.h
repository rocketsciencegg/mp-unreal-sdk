 /*
 * #####################################################################################
 *  Multiplay by Rocket Science Plugin for Unreal Engine Copyright © 2026 Rocket Science Corporation (UK) Ltd.
 * #####################################################################################
 */

#pragma once

#include "OpenAPIQosDiscoveryApiOperations.h"

namespace RocketScienceQos 
{
	class IQosApi
	{
	public:
		virtual ~IQosApi() {};

		virtual FHttpRequestPtr ListServers(const ListServersRequest& Request, const FListServersDelegate& Delegate = FListServersDelegate()) = 0;
	};
		
}