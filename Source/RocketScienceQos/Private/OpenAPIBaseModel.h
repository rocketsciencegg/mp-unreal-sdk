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

#pragma once

#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "Serialization/JsonWriter.h"
#include "Dom/JsonObject.h"
#include "HttpRetrySystem.h"
#include "Containers/Ticker.h"
#include "Runtime/Launch/Resources/Version.h"
#include "Utils/JsonHelpers.h"

namespace RocketScienceQos
{

using namespace FHttpRetrySystem;

#if ENGINE_MAJOR_VERSION == 4 && ENGINE_MINOR_VERSION == 22
// This typedef was added to the engine in UE 4.23, we are replicate it here for compatibility.
typedef TSharedRef<class IHttpRequest> FHttpRequestRef;
typedef TSharedRef<class IHttpResponse, ESPMode::ThreadSafe> FHttpResponseRef;
#endif

#if ENGINE_MAJOR_VERSION == 5
struct ROCKETSCIENCEQOS_API HttpRetryManager : public FManager, public FTSTickerObjectBase
{
	using FManager::FManager;

	bool Tick(float DeltaTime) final;
};
#else
struct ROCKETSCIENCEQOS_API HttpRetryManager : public FManager, public FTickerObjectBase
{
	using FManager::FManager;

	bool Tick(float DeltaTime) final;
};
#endif

struct ROCKETSCIENCEQOS_API HttpRetryParams
{
	HttpRetryParams(
		const FRetryLimitCountSetting& InRetryLimitCountOverride = FRetryLimitCountSetting(),
		const FRetryTimeoutRelativeSecondsSetting& InRetryTimeoutRelativeSecondsOverride = FRetryTimeoutRelativeSecondsSetting(),
		const FRetryResponseCodes& InRetryResponseCodes = FRetryResponseCodes(),
		const FRetryVerbs& InRetryVerbs = FRetryVerbs(),
		const FRetryDomainsPtr& InRetryDomains = FRetryDomainsPtr()
	);

	FRetryLimitCountSetting              RetryLimitCountOverride;
	FRetryTimeoutRelativeSecondsSetting  RetryTimeoutRelativeSecondsOverride;
	FRetryResponseCodes					 RetryResponseCodes;
	FRetryVerbs                          RetryVerbs;
	FRetryDomainsPtr					 RetryDomains;
};

class ROCKETSCIENCEQOS_API Model : public IJsonReadable, public IJsonWritable
{
public:
	virtual ~Model() {}
	virtual void WriteJson(JsonWriter& Writer) const = 0;
	virtual bool FromJson(const TSharedPtr<FJsonValue>& JsonValue) = 0;
};

class ROCKETSCIENCEQOS_API Request
{
public:
	virtual ~Request() {}
	virtual void SetupHttpRequest(const FHttpRequestRef& HttpRequest) const = 0;
	virtual FString ComputePath() const = 0;

	/* Enables retry and optionally sets a retry policy for this request */
	void SetShouldRetry(const HttpRetryParams& Params = HttpRetryParams()) { RetryParams = Params; }
	const TOptional<HttpRetryParams>& GetRetryParams() const { return RetryParams; }

private:
	TOptional<HttpRetryParams> RetryParams;
};

class ROCKETSCIENCEQOS_API Response
{
public:
	virtual ~Response() {}
	virtual bool FromJson(const TSharedPtr<FJsonValue>& JsonValue) = 0;

	void SetSuccessful(bool InSuccessful) { Successful = InSuccessful; }
	bool IsSuccessful() const { return Successful; }

	virtual void SetHttpResponseCode(EHttpResponseCodes::Type InHttpResponseCode);
	EHttpResponseCodes::Type GetHttpResponseCode() const { return ResponseCode; }

	void SetResponseString(const FString& InResponseString) { ResponseString = InResponseString; }
	const FString& GetResponseString() const { return ResponseString; }

	void SetHttpResponse(const FHttpResponsePtr& InHttpResponse) { HttpResponse = InHttpResponse; }
	const FHttpResponsePtr& GetHttpResponse() const { return HttpResponse; }

private:
	bool Successful;
	EHttpResponseCodes::Type ResponseCode;
	FString ResponseString;
	FHttpResponsePtr HttpResponse;
};

}
