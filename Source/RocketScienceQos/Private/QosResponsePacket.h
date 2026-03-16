#pragma once

namespace RocketScienceQos 
{	
	class FQosResponsePacket
	{
private:	
	
		constexpr static uint8 RESPONSE_MAGIC = 0x95;
		constexpr static int BYTE_LENGTH = 13;

		uint16 VerAndFlow;
		uint8 Sequence;
		uint16 Identifier;
		uint64 SentTimestamp;
		uint64 ReceivedTimestamp;

public:

		FQosResponsePacket();
		bool FromBytes(const TArray<uint8>& Bytes);
		void SetReceivedTimestamp(const uint64 InReceivedTimestamp);
		uint16 GetIdentifier() const;
		uint64 GetSentTimestamp() const;
		uint64 GetReceivedTimestamp() const;
		uint8 GetSequence() const;
	};
}