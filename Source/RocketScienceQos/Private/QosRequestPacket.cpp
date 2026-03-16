/*
* #####################################################################################
*  Rocket Science Multiplay Plugin for Unreal Engine Copyright © 2026 Rocket Science Group
* #####################################################################################
*/


#include "QosRequestPacket.h"

namespace RocketScienceQos
{
	FQosRequestPacket::FQosRequestPacket(const FString& InTitle, uint8 InSequence)
		: Title(InTitle), Sequence(InSequence)
	{
	}

    TArray<uint8> FQosRequestPacket::ToBytes() const
    {
		TArray<uint8> Bytes;

		// Data Spec: https://github.com/rocketsciencegg/mp-suite-qos-server/blob/main/docs/PROTOCOL.md

		// First byte: Magic number (0x59)
		Bytes.Add(REQUEST_MAGIC);

		// Second byte: Version in top 4 bits, flow type in bottom 4 bits
		Bytes.Add(VER_AND_FLOW);

		// Title: UTF-8 encoded string prefixed with length byte
		TArray<uint8> TitleBytes;
		FTCHARToUTF8 Converter(*Title);
		TitleBytes.Append(reinterpret_cast<const uint8*>(Converter.Get()), Converter.Length());
		uint8 TitleLength = static_cast<uint8>(TitleBytes.Num() + 1); // +1 for size byte
		Bytes.Add(TitleLength);
		Bytes.Append(TitleBytes);


		// --CUSTOM FIELDS--

		// Sequence: 1 byte
		Bytes.Add(Sequence);

		// Identifier: 2 bytes, little-endian
		for (int i = 0; i < 2; ++i)
		{
			Bytes.Add((Identifier >> (i * 8)) & 0xFF);
		}

		// Timestamp: 8 bytes, little-endian
		for (int i = 0; i < 8; ++i)
		{
			Bytes.Add((Timestamp >> (i * 8)) & 0xFF);
		}

		return Bytes;
    }

	void FQosRequestPacket::SetTimestamp(const uint64 InTimestamp)
	{
		Timestamp = InTimestamp;
	}

	void FQosRequestPacket::SetIdentifier(uint16 InIdentifier)
	{
		Identifier = InIdentifier;
	}

	uint16 FQosRequestPacket::GetIdentifier() const
	{
		return Identifier;
	}

	uint64 FQosRequestPacket::GetTimestamp() const
	{
		return Timestamp;
	}

}

