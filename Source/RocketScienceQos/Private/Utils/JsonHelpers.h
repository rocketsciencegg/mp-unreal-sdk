 /*
 * #####################################################################################
 *  Unity Gaming Services Plugin for Unreal Engine Copyright © 2025 Unity Technologies.
 * #####################################################################################
 */

#pragma once

#include "CoreMinimal.h"
#include "Dom/JsonValue.h"
#include "Dom/JsonObject.h"
#include "Misc/Base64.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonWriter.h"
#include "Serialization/JsonSerializer.h"
#include "Policies/CondensedJsonPrintPolicy.h"

namespace RocketScienceQos
{
	using JsonWriter = TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>>;

	class IJsonWritable
	{
	public:
		virtual ~IJsonWritable() {}
		virtual void WriteJson(JsonWriter& Writer) const = 0;
	};

	class IJsonReadable
	{
	public:
		virtual ~IJsonReadable() {}
		virtual bool FromJson(const TSharedPtr<FJsonValue>& JsonValue) = 0;
	};

	// Decodes Base64Url encoded strings, see https://en.wikipedia.org/wiki/Base64#Variants_summary_table
	template<typename T>
	bool Base64UrlDecode(const FString& Base64String, T& Value)
	{
		FString TmpCopy(Base64String);
		TmpCopy.ReplaceInline(TEXT("-"), TEXT("+"));
		TmpCopy.ReplaceInline(TEXT("_"), TEXT("/"));

		return FBase64::Decode(TmpCopy, Value);
	}

	// Encodes strings in Base64Url, see https://en.wikipedia.org/wiki/Base64#Variants_summary_table
	template<typename T>
	FString Base64UrlEncode(const T& Value)
	{
		FString Base64String = FBase64::Encode(Value);
		Base64String.ReplaceInline(TEXT("+"), TEXT("-"));
		Base64String.ReplaceInline(TEXT("/"), TEXT("_"));
		return Base64String;
	}

	template<typename T>
	inline FStringFormatArg ToStringFormatArg(const T& Value)
	{
		return FStringFormatArg(Value);
	}

	inline FStringFormatArg ToStringFormatArg(const FDateTime& Value)
	{
		return FStringFormatArg(Value.ToIso8601());
	}

	inline FStringFormatArg ToStringFormatArg(const FGuid& Value)
	{
		return FStringFormatArg(Value.ToString(EGuidFormats::DigitsWithHyphens).ToLower());
	}

	inline FStringFormatArg ToStringFormatArg(const TArray<uint8>& Value)
	{
		return FStringFormatArg(Base64UrlEncode(Value));
	}

	template<typename T, typename std::enable_if<!std::is_base_of<IJsonWritable, T>::value, int>::type = 0>
	inline FString ToString(const T& Value)
	{
		return FString::Format(TEXT("{0}"), {ToStringFormatArg(Value)});
	}

	inline FString ToString(const FString& Value)
	{
		return Value;
	}

	inline FString ToString(bool Value)
	{
		return Value ? TEXT("true") : TEXT("false");
	}

	inline FStringFormatArg ToStringFormatArg(bool Value)
	{
		return FStringFormatArg(ToString(Value));
	}

	inline FString ToString(const TArray<uint8>& Value)
	{
		return Base64UrlEncode(Value);
	}

	inline FString ToString(const IJsonWritable& Value)
	{
		FString String;
		JsonWriter Writer = TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&String);
		Value.WriteJson(Writer);
		Writer->Close();
		return String;
	}

	inline void WriteJsonValue(JsonWriter& Writer, const TSharedPtr<FJsonValue>& Value)
	{
		if (Value.IsValid())
		{
			FJsonSerializer::Serialize(Value.ToSharedRef(), "", Writer, false);
		}
		else
		{
			Writer->WriteObjectStart();
			Writer->WriteObjectEnd();
		}
	}

	inline void WriteJsonValue(JsonWriter& Writer, const TSharedPtr<FJsonObject>& Value)
	{
		if (Value.IsValid())
		{
			FJsonSerializer::Serialize(Value.ToSharedRef(), Writer, false);
		}
		else
		{
			Writer->WriteObjectStart();
			Writer->WriteObjectEnd();
		}
	}

	inline void WriteJsonValue(JsonWriter& Writer, const TArray<uint8>& Value)
	{
		Writer->WriteValue(ToString(Value));
	}

	inline void WriteJsonValue(JsonWriter& Writer, const FDateTime& Value)
	{
		Writer->WriteValue(Value.ToIso8601());
	}

	inline void WriteJsonValue(JsonWriter& Writer, const FGuid& Value)
	{
		Writer->WriteValue(Value.ToString(EGuidFormats::DigitsWithHyphens));
	}

	inline void WriteJsonValue(JsonWriter& Writer, const IJsonWritable& Value)
	{
		Value.WriteJson(Writer);
	}

	template<typename T, typename std::enable_if<!std::is_base_of<IJsonWritable, T>::value, int>::type = 0>
	inline void WriteJsonValue(JsonWriter& Writer, const T& Value)
	{
		Writer->WriteValue(Value);
	}

	template<typename T>
	inline void WriteJsonValue(JsonWriter& Writer, const TArray<T>& Value)
	{
		Writer->WriteArrayStart();
		for (const auto& Element : Value)
		{
			WriteJsonValue(Writer, Element);
		}
		Writer->WriteArrayEnd();
	}

	template<typename T>
	inline void WriteJsonValue(JsonWriter& Writer, const TMap<FString, T>& Value)
	{
		Writer->WriteObjectStart();
		for (const auto& It : Value)
		{
			Writer->WriteIdentifierPrefix(It.Key);
			WriteJsonValue(Writer, It.Value);
		}
		Writer->WriteObjectEnd();
	}

	inline bool TryGetJsonValue(const TSharedPtr<FJsonValue>& JsonValue, FString& Value)
	{
		FString TmpValue;
		if (JsonValue->TryGetString(TmpValue))
		{
			Value = TmpValue;
			return true;
		}
		else
			return false;
	}

	bool ParseDateTime(const FString& DateTimeString, FDateTime& OutDateTime);

	/* Type-specific checks */

	//DateTime 
	inline bool TryGetJsonValue(const TSharedPtr<FJsonValue>& JsonValue, FDateTime& Value)
	{
		if (JsonValue->Type == EJson::String)
		{
			FString TmpValue;
			if (JsonValue->TryGetString(TmpValue))
			{
				return ParseDateTime(TmpValue, Value);
			}
		}

		return false;
	}

	//Guid
	inline bool TryGetJsonValue(const TSharedPtr<FJsonValue>& JsonValue, FGuid& Value)
	{
		if (JsonValue->Type == EJson::String)
		{
			FString TmpValue;
			if (JsonValue->TryGetString(TmpValue))
			{
				return FGuid::Parse(TmpValue, Value);
			}
		}
		
		return false;
	}

	//Bool
	inline bool TryGetJsonValue(const TSharedPtr<FJsonValue>& JsonValue, bool& Value)
	{
		if (JsonValue->Type == EJson::Boolean)
		{
			bool TmpValue;
			if (JsonValue->TryGetBool(TmpValue))
			{
				Value = TmpValue;
				return true;
			}
		}
		
		return false;
	}

	//Int32
	inline bool TryGetJsonValue(const TSharedPtr<FJsonValue>& JsonValue, int32& Value)
	{
		if (JsonValue->Type == EJson::Number)
		{
			int32 TmpValue;
			if (JsonValue->TryGetNumber(TmpValue))
			{
				Value = TmpValue;
				return true;
			}
		}

		return false;
	}

	//JsonValue
	inline bool TryGetJsonValue(const TSharedPtr<FJsonValue>& JsonValue, TSharedPtr<FJsonValue>& JsonObjectValue)
	{
		JsonObjectValue = JsonValue;
		return true;
	}

	//JsonObject
	inline bool TryGetJsonValue(const TSharedPtr<FJsonValue>& JsonValue, TSharedPtr<FJsonObject>& JsonObjectValue)
	{
		if (JsonValue->Type == EJson::Object)
		{
			const TSharedPtr<FJsonObject>* Object;
			if (JsonValue->TryGetObject(Object))
			{
				JsonObjectValue = *Object;
				return true;
			}
		}

		return false;
	}

	//Array (encoded)
	inline bool TryGetJsonValue(const TSharedPtr<FJsonValue>& JsonValue, TArray<uint8>& Value)
	{
		if (JsonValue->Type == EJson::Array)
		{
			FString TmpValue;
			if (JsonValue->TryGetString(TmpValue))
			{
				Base64UrlDecode(TmpValue, Value);
				return true;
			}
		}
		
		return false;
	}

	//JsonReadable
	inline bool TryGetJsonValue(const TSharedPtr<FJsonValue>& JsonValue, IJsonReadable& Value)
	{
		return Value.FromJson(JsonValue);
	}

	//Int32 (templated)
	template<typename T, typename std::enable_if<!std::is_base_of<IJsonReadable, T>::value, int>::type = 0>
	inline bool TryGetJsonValue(const TSharedPtr<FJsonValue>& JsonValue, T& Value)
	{
		if (JsonValue->Type == EJson::Number)
		{
			T TmpValue;

			if (JsonValue->TryGetNumber(TmpValue))
			{
				Value = TmpValue;
				return true;
			}
		}

		return false;
	}

	//Array (templated)
	template<typename T>
	inline bool TryGetJsonValue(const TSharedPtr<FJsonValue>& JsonValue, TArray<T>& ArrayValue)
	{
		if (JsonValue->Type == EJson::Array)
		{
			const TArray<TSharedPtr<FJsonValue>>* JsonArray;
			if (JsonValue->TryGetArray(JsonArray))
			{
				bool ParseSuccess = true;
				const int32 Count = JsonArray->Num();
				ArrayValue.Reset(Count);
				for (int i = 0; i < Count; i++)
				{
					T TmpValue;
					ParseSuccess &= TryGetJsonValue((*JsonArray)[i], TmpValue);
					ArrayValue.Emplace(MoveTemp(TmpValue));
				}
				return ParseSuccess;
			}
		}
		
		return false;
	}

	//Map (templated)
	template<typename T>
	inline bool TryGetJsonValue(const TSharedPtr<FJsonValue>& JsonValue, TMap<FString, T>& MapValue)
	{
		if (JsonValue->Type == EJson::Object)
		{
			const TSharedPtr<FJsonObject>* Object;
			if (JsonValue->TryGetObject(Object))
			{
				MapValue.Reset();
				bool ParseSuccess = true;
				for (const auto& It : (*Object)->Values)
				{
					T TmpValue;
					ParseSuccess &= TryGetJsonValue(It.Value, TmpValue);
					MapValue.Emplace(It.Key, MoveTemp(TmpValue));
				}
				return ParseSuccess;
			}
		}

		return false;
	}

	/* end */

	template<typename T>
	inline bool TryGetJsonValue(const TSharedPtr<FJsonObject>& JsonObject, const FString& Key, T& Value)
	{
		const TSharedPtr<FJsonValue> JsonValue = JsonObject->TryGetField(Key);
		if (JsonValue.IsValid() && !JsonValue->IsNull())
		{
			//Grab the nested values, but return true since the upmost value was non-null
			TryGetJsonValue(JsonValue, Value);
			return true;
		}
		return false;
	}

	template<typename T>
	inline bool TryGetJsonValue(const TSharedPtr<FJsonObject>& JsonObject, const FString& Key, TOptional<T>& OptionalValue)
	{
		if (JsonObject->HasField(Key))
		{
			T Value;
			if (TryGetJsonValue(JsonObject, Key, Value))
			{
				OptionalValue = Value;
				return true;
			}
			else
				return false;
		}
		return true; // Absence of optional value is not a parsing error
	}

} // namespace RocketScienceQos