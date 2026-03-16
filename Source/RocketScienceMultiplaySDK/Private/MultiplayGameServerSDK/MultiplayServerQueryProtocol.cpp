/*
 * #####################################################################################
 *  Rocket Science Multiplay Plugin for Unreal Engine Copyright © 2026 Rocket Science Group
 * #####################################################################################
 */

#include "MultiplayServerQueryProtocol.h"
#include "MultiplayGameServerSDKLog.h"

namespace RocketScienceMultiplaySDK
{
	// This function is a modified version of operator<< from Engine/Source/Runtime/Core/Private/Containers/String.cpp:1386.
	// The logic has been modified to convert from TCHAR to UTF8CHAR because SQP supports UTF-8 strings.
	static inline void SerializeSQPStringForServerQuery(FArchive& Ar, FString& SQPString)
	{
		if (Ar.IsLoading())
		{
			uint8 SaveNum = 0;
			Ar << SaveNum;

			// Protect against network packets allocating too much memory
			int64 MaxSerializeSize = Ar.GetMaxSerializeSize();
			if ((MaxSerializeSize > 0) && (SaveNum > MaxSerializeSize))
			{
#if ENGINE_MAJOR_VERSION == 4 && ENGINE_MINOR_VERSION <= 25
				Ar.ArIsCriticalError = true;
				Ar.ArIsError = true;
#else // UE 4.26+ uses FArchiveState::SetCriticalError()
				Ar.SetCriticalError();
#endif
				UE_LOG(LogRSMultiplayGameServerSDK, Error, TEXT("String is too large (Size: %i, Max: %i)"), SaveNum, MaxSerializeSize);
				
				return;
			}

			if (SaveNum)
			{
				TArray<UTF8CHAR> Buffer;
				Buffer.Empty(SaveNum + 1);
				Buffer.AddUninitialized(SaveNum + 1);

				Ar.Serialize(Buffer.GetData(), SaveNum * sizeof(UTF8CHAR));

				// Ensure the string has a null terminator
				Buffer[SaveNum] = UTF8CHAR('\0');

				SQPString = UTF8_TO_TCHAR(Buffer.GetData());
			}
			// Throw away empty string.
			else
			{
				SQPString.GetCharArray().Empty();
			}
		}
		else
		{
			FTCHARToUTF8 ConvertedString(*SQPString);

			uint8 Len = ConvertedString.Length();
			Ar << Len;

			if (Len)
			{
				Ar.Serialize((void*)ConvertedString.Get(), sizeof(UTF8CHAR) * Len);
			}
		}
	}

	FArchive& operator<<(FArchive& Ar, FSQPHeader& Data)
	{
		Ar << Data.Type;
		Ar << Data.ChallengeToken;
		return Ar;
	}

	FArchive& operator<<(FArchive& Ar, FSQPChallengePacket& Data)
	{
#if PLATFORM_LITTLE_ENDIAN
		// Byte swapping is necessary for our int values to be read correctly
		// SQP reads Big Endianness
		Ar.SetByteSwapping(true);
#endif

		Ar << Data.Header;
		return Ar;
	}

	FArchive& operator<<(FArchive& Ar, FSQPQueryRequestPacket& Data)
	{
#if PLATFORM_LITTLE_ENDIAN
		// Byte swapping is necessary for our int values to be read correctly
		// SQP reads Big Endianness
		Ar.SetByteSwapping(true);
#endif

		Ar << Data.Header;
		Ar << Data.Version;
		Ar << Data.RequestedChunks;
		return Ar;
	}

	FArchive& operator<<(FArchive& Ar, FSQPQueryResponseHeader& Data)
	{
		Ar << Data.Header;
		Ar << Data.Version;
		Ar << Data.CurrentPacket;
		Ar << Data.LastPacket;
		Ar << Data.PacketLength;
		return Ar;
	}

	FArchive& operator<<(FArchive& Ar, FSQPServerInfoData& Data)
	{
		Ar << Data.CurrentPlayers;
		Ar << Data.MaxPlayers;
		SerializeSQPStringForServerQuery(Ar, Data.ServerName);
		SerializeSQPStringForServerQuery(Ar, Data.GameType);
		SerializeSQPStringForServerQuery(Ar, Data.BuildId);
		SerializeSQPStringForServerQuery(Ar, Data.Map);
		Ar << Data.Port;
		return Ar;
	}

	FArchive& operator<<(FArchive& Ar, FSQPQueryResponsePacket& Data)
	{
		int64 ServerInfoChunkStartPos = 0;

		bool bRequestedServerInfo = ((Data.RequestedChunks & static_cast<uint8>(ESQPChunkType::ServerInfo)) > 0);

#if PLATFORM_LITTLE_ENDIAN
		// Byte swapping is necessary for our int values to be read correctly
		// SQP reads Big Endianness
		Ar.SetByteSwapping(true);
#endif

		if (Ar.IsLoading())
		{
			Ar << Data.QueryHeader;
			Ar << Data.ServerInfoChunkLength;

			if (bRequestedServerInfo)
			{
				Ar << Data.ServerInfoData;
			}
		}
		else
		{
			Ar << Data.QueryHeader;

			// Place a marker where QueryHeader.Length is, We will overwrite the PacketLength later
			int64 QueryHeaderLengthPos = Ar.TotalSize() - sizeof(uint16);

			// Place a marker where the start position of the packet is
			int64 StartPos = Ar.TotalSize();

			if (bRequestedServerInfo)
			{
				// Write a placeholder ChunkLength. This will be overwritten later.
				Ar << Data.ServerInfoChunkLength;

				// Place a marker where the Chunk is started
				ServerInfoChunkStartPos = Ar.TotalSize();

				Ar << Data.ServerInfoData;

				// Determine how long the Chunk is
				Data.ServerInfoChunkLength = Ar.TotalSize() - ServerInfoChunkStartPos;
			}

			// Determine how long our packet is, including the chunk lengths
			Data.QueryHeader.PacketLength = Ar.TotalSize() - StartPos;

			// Overwrite QueryHeader.Length
			Ar.Seek(QueryHeaderLengthPos);
			Ar << Data.QueryHeader.PacketLength;

			if (bRequestedServerInfo)
			{
				// Overwrite ServerInfoChunkLength
				Ar.Seek(ServerInfoChunkStartPos - sizeof(Data.ServerInfoChunkLength));
				Ar << Data.ServerInfoChunkLength;
			}
		}

		return Ar;
	}
} // namespace RocketScienceMultiplaySDK