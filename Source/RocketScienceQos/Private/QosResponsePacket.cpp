/*
* #####################################################################################
*  Rocket Science Multiplay Plugin for Unreal Engine Copyright © 2026 Rocket Science Group
* #####################################################################################
*/


#include "QosResponsePacket.h"
#include "RSQosModule.h"

namespace RocketScienceQos
{
	FQosResponsePacket::FQosResponsePacket()
	{
	}

	bool FQosResponsePacket::FromBytes(const TArray<uint8>& Bytes)
	{
		if (Bytes.Num() < BYTE_LENGTH)
		{
			UE_LOG(LogRSQos, Error, TEXT("Failed to parse QoS response packet as byte array is too small!"));
			return false;
		}
		
		// First byte: Magic number (0x95)
		if (Bytes[0] != RESPONSE_MAGIC)
		{
			UE_LOG(LogRSQos, Error, TEXT("Failed to parse QoS response packet as magic byte is invalid!"));
			return false;
		}

		// Second byte: Version in top 4 bits, flow type in bottom 4 bits (currently unused)
		VerAndFlow = Bytes[1];

		// --CUSTOM FIELDS--

		// Sequence: one byte
		Sequence = Bytes[2];

		// Identifier: 2 bytes, little-endian
		Identifier = (static_cast<uint16>(Bytes[3])) | static_cast<uint16>(Bytes[4] << 8);
		
		// Sent Timestamp: 8 bytes, little-endian
		SentTimestamp = 0;
		for (int i = 0; i < 8; i++)
		{
			SentTimestamp |= (static_cast<uint64>(Bytes[5 + i]) << (8 * (i)));
		}

		return true;
	}

	void FQosResponsePacket::SetReceivedTimestamp(const uint64 InReceivedTimestamp)
	{
		ReceivedTimestamp = InReceivedTimestamp;
	}

	uint16 FQosResponsePacket::GetIdentifier() const
	{
		return Identifier;
	}

	uint64 FQosResponsePacket::GetSentTimestamp() const
	{
		return SentTimestamp;
	}

	uint64 FQosResponsePacket::GetReceivedTimestamp() const
	{
		return ReceivedTimestamp;
	}

	uint8 FQosResponsePacket::GetSequence() const
	{
		return Sequence;
	}
}

