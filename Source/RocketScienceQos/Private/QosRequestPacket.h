#pragma once

namespace RocketScienceQos 
{	
	class FQosRequestPacket
	{
private:	
	
		constexpr static uint8 REQUEST_MAGIC = 0x59;
		constexpr static uint8 VER_AND_FLOW = 0x00;

		FString Title;

		uint8 Sequence;
		uint16 Identifier;
		uint64 Timestamp;

public:

		FQosRequestPacket(const FString& InTitle, uint8 InSequence);
		void SetTimestamp(const uint64 InTimestamp);
		void SetIdentifier(uint16 InIdentifier);
		uint64 GetTimestamp() const;
		uint16 GetIdentifier() const;
		TArray<uint8> ToBytes() const;
	};
}