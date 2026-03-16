 /*
 * #####################################################################################
 *  Unity Gaming Services Plugin for Unreal Engine Copyright © 2025 Unity Technologies.
 * #####################################################################################
 */

#include "WrappedQosApi.h"
#include "IQosApi.h"
#include "RSQosModule.h"
#include "TimerManager.h"

namespace RocketScienceQos
{
	FWrappedQosApi::FWrappedQosApi()
	{
		QosApi = MakeUnique<OpenAPIQosDiscoveryApi>();
	}

	// TODO (Jac Griffiths): Re-enable when we have auth available
	//FWrappedQosApi::FWrappedQosApi(UAuthenticationSubsystem* AuthenticationSubsystem)
	//	: AuthenticationSubsystem(AuthenticationSubsystem)
	//{
	//	QosApi = MakeUnique<OpenAPIQosDiscoveryApi>();
	//}

	FHttpRequestPtr FWrappedQosApi::ListServers(const ListServersRequest& Request, const FListServersDelegate& Delegate)
	{
		QosApi->ClearHeaderParams();

		// TODO (Jac Griffiths): Add auth to header when available
		//if (AuthenticationSubsystem->IsSignedIn())
		{
		//	FString AuthToken = AuthenticationSubsystem->GetAccessToken();
		//	QosApi->AddHeaderParam(TEXT("Authorization"), FString::Printf(TEXT("Bearer %s"), *AuthToken));
			QosApi->ListServers(Request, Delegate);
		}
		//else
		/*{
			UE_LOG(LogRSQos, Error, TEXT("You are not signed in to the Authentication Service. Please sign in."));
			ListServersResponse Response;
			Response.SetSuccessful(false);
			Response.SetResponseString(TEXT("You are not signed in to the Authentication Service. Please sign in."));
			Delegate.ExecuteIfBound(Response);
		}*/

		return FHttpRequestPtr();
	}
}

