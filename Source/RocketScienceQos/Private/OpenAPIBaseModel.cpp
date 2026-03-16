/**
 * QoS Discovery Service API
 * API contract for the server discovery service for QoS servers running as part of the QoS Service.
 *
 * #####################################################################################
 *  Multiplay by Rocket Science Plugin for Unreal Engine Copyright © 2026 Rocket Science Corporation (UK) Ltd.
 * #####################################################################################
 * 
 * NOTE: This is a generated source file. Do not modify manually.
 */

#include "OpenAPIBaseModel.h"

namespace RocketScienceQos
{

bool HttpRetryManager::Tick(float DeltaTime)
{
#if (ENGINE_MAJOR_VERSION <= 5 && ENGINE_MINOR_VERSION <= 5) || ENGINE_MAJOR_VERSION <= 4
	FManager::Update();
#endif
	return true;
}

HttpRetryParams::HttpRetryParams(const FRetryLimitCountSetting& InRetryLimitCountOverride /*= FRetryLimitCountSetting()*/,
    const FRetryTimeoutRelativeSecondsSetting& InRetryTimeoutRelativeSecondsOverride /*= FRetryTimeoutRelativeSecondsSetting()*/,
    const FRetryResponseCodes& InRetryResponseCodes /*= FRetryResponseCodes()*/,
    const FRetryVerbs& InRetryVerbs /*= FRetryVerbs()*/,
    const FRetryDomainsPtr& InRetryDomains /*= FRetryDomainsPtr() */)
    : RetryLimitCountOverride(InRetryLimitCountOverride)
    , RetryTimeoutRelativeSecondsOverride(InRetryTimeoutRelativeSecondsOverride)
    , RetryResponseCodes(InRetryResponseCodes)
    , RetryVerbs(InRetryVerbs)
    , RetryDomains(InRetryDomains)
{
}

void Response::SetHttpResponseCode(EHttpResponseCodes::Type InHttpResponseCode)
{
    ResponseCode = InHttpResponseCode;
    SetSuccessful(EHttpResponseCodes::IsOk(InHttpResponseCode));
    if(InHttpResponseCode == EHttpResponseCodes::RequestTimeout)
    {
        SetResponseString(TEXT("Request Timeout"));
    }
}

}
