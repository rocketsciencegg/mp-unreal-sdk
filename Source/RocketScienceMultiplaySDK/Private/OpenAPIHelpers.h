/**
 * Multiplay by Rocket Science SDK Daemon Game Server API
 * The game server API is called by the Multiplay Game Server SDK itself to subscribe to events and report server status.
 *
 * #####################################################################################
 *  Multiplay by Rocket Science Plugin for Unreal Engine Copyright © 2026 Rocket Science Corporation (UK) Ltd.
 * #####################################################################################
 * 
 * NOTE: This is a generated source file. Do not modify manually.
 */

#pragma once

#include "OpenAPIBaseModel.h"

#include "Serialization/JsonSerializer.h"
#include "Dom/JsonObject.h"
#include "Misc/Base64.h"
#include "PlatformHttp.h"
#include "Utils/MultiplayJsonHelpers.h"

class IHttpRequest;

namespace RocketScienceMultiplaySDK
{

//////////////////////////////////////////////////////////////////////////

class ROCKETSCIENCEMULTIPLAYSDK_API HttpFileInput
{
public:
	explicit HttpFileInput(const TCHAR* InFilePath);
	explicit HttpFileInput(const FString& InFilePath);

	// This will automatically set the content type if not already set
    void SetFilePath(const TCHAR* InFilePath);
	void SetFilePath(const FString& InFilePath);

    // Optional if it can be deduced from the FilePath
    void SetContentType(const TCHAR* ContentType);

	HttpFileInput& operator=(const HttpFileInput& Other) = default;
	HttpFileInput& operator=(const FString& InFilePath) { SetFilePath(*InFilePath); return*this; }
	HttpFileInput& operator=(const TCHAR* InFilePath) { SetFilePath(InFilePath); return*this; }

	const FString& GetFilePath() const { return FilePath; }
	const FString& GetContentType() const { return ContentType; }

	// Returns the filename with extension
	FString GetFilename() const;

private:
    FString FilePath;
    FString ContentType;
};

//////////////////////////////////////////////////////////////////////////

class HttpMultipartFormData
{
public:
	void SetBoundary(const TCHAR* InBoundary);
	void SetupHttpRequest(const FHttpRequestRef& HttpRequest);

	void AddStringPart(const TCHAR* Name, const TCHAR* Data);
	void AddJsonPart(const TCHAR* Name, const FString& JsonString);
	void AddBinaryPart(const TCHAR* Name, const TArray<uint8>& ByteArray);
	void AddFilePart(const TCHAR* Name, const HttpFileInput& File);

private:
	void AppendString(const TCHAR* Str);
	const FString& GetBoundary() const;

	mutable FString Boundary;
	TArray<uint8> FormData;

	static const TCHAR* Delimiter;
	static const TCHAR* Newline;
};

//////////////////////////////////////////////////////////////////////////

template<typename T, typename std::enable_if<!std::is_base_of<Model, T>::value, int>::type = 0>
inline FString ToString(const T& Value)
{
	return FString::Format(TEXT("{0}"), { ToStringFormatArg(Value) });
}

inline FString ToString(const Model& Value)
{
	FString String;
	JsonWriter Writer = TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&String);
	Value.WriteJson(Writer);
	Writer->Close();
	return String;
}

template<typename T>
inline FString ToUrlString(const T& Value)
{
	return FPlatformHttp::UrlEncode(ToString(Value));
}

template<typename T>
inline FString CollectionToUrlString(const TArray<T>& Collection, const TCHAR* Separator)
{
	FString Output;
	if(Collection.Num() == 0)
		return Output;

	Output += ToUrlString(Collection[0]);
	for(int i = 1; i < Collection.Num(); i++)
	{
		Output += FString::Format(TEXT("{0}{1}"), { Separator, *ToUrlString(Collection[i]) });
	}
	return Output;
}

template<typename T>
inline FString CollectionToUrlString_csv(const TArray<T>& Collection, const TCHAR* BaseName)
{
	return CollectionToUrlString(Collection, TEXT(","));
}

template<typename T>
inline FString CollectionToUrlString_ssv(const TArray<T>& Collection, const TCHAR* BaseName)
{
	return CollectionToUrlString(Collection, TEXT(" "));
}

template<typename T>
inline FString CollectionToUrlString_tsv(const TArray<T>& Collection, const TCHAR* BaseName)
{
	return CollectionToUrlString(Collection, TEXT("\t"));
}

template<typename T>
inline FString CollectionToUrlString_pipes(const TArray<T>& Collection, const TCHAR* BaseName)
{
	return CollectionToUrlString(Collection, TEXT("|"));
}

template<typename T>
inline FString CollectionToUrlString_multi(const TArray<T>& Collection, const TCHAR* BaseName)
{
	FString Output;
	if(Collection.Num() == 0)
		return Output;

	Output += FString::Format(TEXT("{0}={1}"), { FStringFormatArg(BaseName), ToUrlString(Collection[0]) });
	for(int i = 1; i < Collection.Num(); i++)
	{
		Output += FString::Format(TEXT("&{0}={1}"), { FStringFormatArg(BaseName), ToUrlString(Collection[i]) });
	}
	return Output;
}

}
