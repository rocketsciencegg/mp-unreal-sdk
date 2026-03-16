/*
 * #####################################################################################
 *  Rocket Science Multiplay Plugin for Unreal Engine Copyright © 2026 Rocket Science Group
 * #####################################################################################
 */

#pragma once

#include "CoreMinimal.h"

namespace RocketScienceMultiplaySDK
{
	/** The type of request an SQP query is making when the message type is ChallengeRequest */
	enum class ESQPChunkType : uint8
	{
		ServerInfo = 1 << 0,
		ServerRules = 1 << 1,
		PlayerInfo = 1 << 2,
		TeamInfo = 1 << 3
	};

	/** The type of response field in an SQP query response packet for Players or Teams */
	enum class ESQPDynamicType : uint8
	{
		Byte = 0,
		Uint16 = 1,
		Uint32 = 2,
		Uint64 = 3,
		String = 4
	};

	/** The top-level message type for an SQP packet */
	enum class ESQPMessageType : uint8
	{
		ChallengeRequest = 0,
		ChallengeResponse = 0,
		QueryRequest = 1,
		QueryResponse = 1
	};

	/** A struct to serialize/deserialize all SQP packet headers */
	struct FSQPHeader
	{
		uint8 Type;
		uint32 ChallengeToken;

		/** Serialization operator. */
		friend FArchive& operator<<(FArchive& Ar, FSQPHeader& Data);
	};

	/**
	** A struct to serialize/deserialize all SQP challenge packets
	** ChallengeRequest and ChallengeResponse packets follow the same format, only define one struct
	*/
	struct FSQPChallengePacket
	{
		FSQPHeader Header;

		/** Serialization operator. */
		friend FArchive& operator<<(FArchive& Ar, FSQPChallengePacket& Data);
	};

	/** A struct to serialize/deserialize SQP QueryRequest packets */
	struct FSQPQueryRequestPacket
	{
		FSQPHeader Header;
		uint16 Version;
		uint8 RequestedChunks;

		/** Serialization operator. */
		friend FArchive& operator<<(FArchive& Ar, FSQPQueryRequestPacket& Data);
	};

	/** A struct to serialize/deserialize the header for SQP QueryResponse packets */
	struct FSQPQueryResponseHeader
	{
		FSQPHeader Header;
		uint16 Version;
		uint8 CurrentPacket;
		uint8 LastPacket;
		uint16 PacketLength;

		/** Serialization operator. */
		friend FArchive& operator<<(FArchive& Ar, FSQPQueryResponseHeader& Data);
	};

	/** A struct to serialize/deserialize the info for SQP QueryResponse packets */
	struct FSQPServerInfoData
	{
		uint16 CurrentPlayers;
		uint16 MaxPlayers;
		FString ServerName;
		FString GameType;
		FString BuildId;
		FString Map;
		uint16 Port;

		/** Serialization operator. */
		friend FArchive& operator<<(FArchive& Ar, FSQPServerInfoData& Data);
	};

	/** A struct to serialize/deserialize SQP QueryResponse packets with a ServerInfo ChunkType */
	struct FSQPQueryResponsePacket
	{
		uint8 RequestedChunks; // Not sent in response, only used to inform how we construct the response
		FSQPQueryResponseHeader QueryHeader;
		uint32 ServerInfoChunkLength;
		FSQPServerInfoData ServerInfoData;

		/** Serialization operator. */
		friend FArchive& operator<<(FArchive& Ar, FSQPQueryResponsePacket& Data);
	};
} // namespace RocketScienceMultiplaySDK