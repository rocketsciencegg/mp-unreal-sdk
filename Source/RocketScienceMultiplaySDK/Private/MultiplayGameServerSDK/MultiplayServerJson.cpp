/*
 * #####################################################################################
 *  Rocket Science Multiplay Plugin for Unreal Engine Copyright © 2026 Rocket Science Group
 * #####################################################################################
 */

#include "MultiplayServerJson.h"

namespace RocketScienceMultiplaySDK
{
	FMultiplayServerJson::~FMultiplayServerJson()
	{
	}

	bool FMultiplayServerJson::FromJson(const TSharedPtr<FJsonValue>& JsonValue)
	{
		const TSharedPtr<FJsonObject>* Object;
		if (!JsonValue->TryGetObject(Object))
			return false;

		bool bParseSuccess = true;

		// serverID can be represented as either a string or an integer. 
		// Clanforge is flexible in defining server.json values as either string or integer.
		// Multiplay self-serve defines all server.json values as strings.
		FString ServerIdAsString;
		int64 ServerIdAsInteger;
		if (TryGetJsonValue(*Object, TEXT("serverID"), ServerIdAsString))
		{
			const TCHAR* Start = *ServerIdAsString;
			TCHAR* End;

			ServerId = FCString::Strtoi64(Start, &End, 0);

			if ((Start == End) || (*End != TEXT('\0')) || (errno == ERANGE))
			{
				bParseSuccess = false;
			}
		}
		else if (TryGetJsonValue(*Object, TEXT("serverID"), ServerIdAsInteger))
		{
			ServerId = ServerIdAsInteger;
		}
		else
		{
			bParseSuccess = false;
		}

		if (TryGetJsonValue(*Object, TEXT("allocatedUUID"), AllocationId))
		{
		}
		else
		{
			bParseSuccess = false;
		}

		// queryPort can be represented as either a string or an integer. 
		// Clanforge is flexible in defining server.json values as either string or integer.
		// Multiplay self-serve defines all server.json values as strings.
		FString QueryPortAsString;
		int64 QueryPortAsInteger;
		if (TryGetJsonValue(*Object, TEXT("queryPort"), QueryPortAsString))
		{
			const TCHAR* Start = *QueryPortAsString;
			TCHAR* End;

			QueryPort = FCString::Strtoi64(Start, &End, 0);

			if ((Start == End) || (*End != TEXT('\0')) || (errno == ERANGE))
			{
				bParseSuccess = false;
			}
		}
		else if (TryGetJsonValue(*Object, TEXT("queryPort"), QueryPortAsInteger))
		{
			QueryPort = QueryPortAsInteger;
		}
		else
		{
			bParseSuccess = false;
		}

		if (TryGetJsonValue(*Object, TEXT("ip"), Ip))
		{
		}
		else
		{
			bParseSuccess = false;
		}

		// port can be represented as either a string or an integer. 
		// Clanforge is flexible in defining server.json values as either string or integer.
		// Multiplay self-serve defines all server.json values as strings.
		FString PortAsString;
		int64 PortAsInteger;
		if (TryGetJsonValue(*Object, TEXT("port"), PortAsString))
		{
			const TCHAR* Start = *PortAsString;
			TCHAR* End;

			Port = FCString::Strtoi64(Start, &End, 0);

			if ((Start == End) || (*End != TEXT('\0')) || (errno == ERANGE))
			{
				bParseSuccess = false;
			}
		}
		else if (TryGetJsonValue(*Object, TEXT("port"), PortAsInteger))
		{
			Port = PortAsInteger;
		}
		else
		{
			bParseSuccess = false;
		}

        if (TryGetJsonValue(*Object, TEXT("serverLogDir"), ServerLogDirectory))
        {
        }
        else
        {
            bParseSuccess = false;
        }

		return bParseSuccess;
	}
} // namespace RocketScienceMultiplaySDK