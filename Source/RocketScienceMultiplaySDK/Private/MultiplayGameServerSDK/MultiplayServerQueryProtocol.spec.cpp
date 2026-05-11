/*
 * #####################################################################################
 *  Rocket Science Multiplay Plugin for Unreal Engine Copyright © 2026 Rocket Science Group
 * #####################################################################################
 */

#include "Tests/AutomationCommon.h"
#include "MultiplayGameServerSDK/MultiplayServerQueryProtocol.h"
#include "Serialization/ArrayWriter.h"
#include "Serialization/ArrayReader.h"

#if WITH_AUTOMATION_TESTS
BEGIN_DEFINE_SPEC(FRSMultiplayServerQueryProtocolSpec, "MultiplayGameServerSDK.ServerQueryProtocol",
	EAutomationTestFlags::ProductFilter | 
	EAutomationTestFlags::EditorContext | 
	EAutomationTestFlags::ClientContext | 
	EAutomationTestFlags::ServerContext | 
	EAutomationTestFlags::CommandletContext)
END_DEFINE_SPEC(FRSMultiplayServerQueryProtocolSpec)

void FRSMultiplayServerQueryProtocolSpec::Define()
{
	Describe("FSQPHeader", [this]()
		{
			Describe("operator<<", [this]()
				{
					It("should properly serialize and deserialize its member variables.", [this]()
						{
							TSharedPtr<FArrayWriter> ArrayWriter = MakeShared<FArrayWriter>(false);
							TSharedPtr<FArrayReader> ArrayReader = MakeShared<FArrayReader>(false);

							RocketScienceMultiplaySDK::FSQPHeader Input = {};
							Input.Type = static_cast<uint8>(RocketScienceMultiplaySDK::ESQPMessageType::ChallengeRequest);
							Input.ChallengeToken = 0xdeadbeef;

							RocketScienceMultiplaySDK::FSQPHeader Output = {};

							*ArrayWriter << Input;
							ArrayReader->Append(ArrayWriter->GetData(), ArrayWriter->Num());
							*ArrayReader << Output;

							TestFalse("FArrayWriter::IsError()", ArrayWriter->IsError());
							TestFalse("FArrayReader::IsError()", ArrayReader->IsError());
							TestEqual("FSQPHeader::Type", Output.Type, Input.Type);
							TestEqual("FSQPHeader::ChallengeToken", Output.ChallengeToken, Input.ChallengeToken);
						});
				});
		});

	Describe("FSQPChallengePacket", [this]()
		{
			Describe("operator<<", [this]()
				{
					It("should properly serialize and deserialize its member variables.", [this]()
						{
							TSharedPtr<FArrayWriter> ArrayWriter = MakeShared<FArrayWriter>(false);
							TSharedPtr<FArrayReader> ArrayReader = MakeShared<FArrayReader>(false);

							RocketScienceMultiplaySDK::FSQPHeader InputHeader = {};
							InputHeader.Type = static_cast<uint8>(RocketScienceMultiplaySDK::ESQPMessageType::ChallengeRequest);
							InputHeader.ChallengeToken = 0xdeadbeef;

							RocketScienceMultiplaySDK::FSQPChallengePacket Input = {};
							Input.Header = InputHeader;

							RocketScienceMultiplaySDK::FSQPChallengePacket Output = {};

							*ArrayWriter << Input;
							ArrayReader->Append(ArrayWriter->GetData(), ArrayWriter->Num());
							*ArrayReader << Output;

							TestFalse("FArrayWriter::IsError()", ArrayWriter->IsError());
							TestFalse("FArrayReader::IsError()", ArrayReader->IsError());
							TestEqual("FSQPChallengePacket::Header::Type", Output.Header.Type, Input.Header.Type);
							TestEqual("FSQPChallengePacket::Header::ChallengeToken", Output.Header.ChallengeToken, Input.Header.ChallengeToken);
						});
				});
		});

	Describe("FSQPQueryRequestPacket", [this]()
		{
			Describe("operator<<", [this]()
				{
					It("should properly serialize and deserialize its member variables.", [this]()
						{
							TSharedPtr<FArrayWriter> ArrayWriter = MakeShared<FArrayWriter>(false);
							TSharedPtr<FArrayReader> ArrayReader = MakeShared<FArrayReader>(false);

							RocketScienceMultiplaySDK::FSQPHeader InputHeader = {};
							InputHeader.Type = static_cast<uint8>(RocketScienceMultiplaySDK::ESQPMessageType::QueryRequest);
							InputHeader.ChallengeToken = 0xdeadbeef;

							RocketScienceMultiplaySDK::FSQPQueryRequestPacket Input = {};
							Input.Header = InputHeader;
							Input.Version = 0xdead;
							Input.RequestedChunks = 0x99;

							RocketScienceMultiplaySDK::FSQPQueryRequestPacket Output = {};

							*ArrayWriter << Input;
							ArrayReader->Append(ArrayWriter->GetData(), ArrayWriter->Num());
							*ArrayReader << Output;

							TestFalse("FArrayWriter::IsError()", ArrayWriter->IsError());
							TestFalse("FArrayReader::IsError()", ArrayReader->IsError());
							TestEqual("FSQPQueryRequestPacket::Header::Type", Output.Header.Type, Input.Header.Type);
							TestEqual("FSQPQueryRequestPacket::Header::ChallengeToken", Output.Header.ChallengeToken, Input.Header.ChallengeToken);
							TestEqual("FSQPQueryRequestPacket::Version", Output.Version, Input.Version);
							TestEqual("FSQPQueryRequestPacket::RequestedChunks", Output.RequestedChunks, Input.RequestedChunks);
						});
				});
		});

	Describe("FSQPQueryResponseHeader", [this]()
		{
			Describe("operator<<", [this]()
				{
					It("should properly serialize and deserialize its member variables.", [this]()
						{
							TSharedPtr<FArrayWriter> ArrayWriter = MakeShared<FArrayWriter>(false);
							TSharedPtr<FArrayReader> ArrayReader = MakeShared<FArrayReader>(false);

							RocketScienceMultiplaySDK::FSQPHeader InputHeader = {};
							InputHeader.Type = static_cast<uint8>(RocketScienceMultiplaySDK::ESQPMessageType::QueryRequest);
							InputHeader.ChallengeToken = 0xdeadbeef;

							RocketScienceMultiplaySDK::FSQPQueryResponseHeader Input = {};
							Input.Header = InputHeader;
							Input.Version = 0xdead;
							Input.CurrentPacket = 0x01;
							Input.LastPacket = 0x99;
							Input.PacketLength = 0xbeef;

							RocketScienceMultiplaySDK::FSQPQueryResponseHeader Output = {};

							*ArrayWriter << Input;
							ArrayReader->Append(ArrayWriter->GetData(), ArrayWriter->Num());
							*ArrayReader << Output;

							TestFalse("FArrayWriter::IsError()", ArrayWriter->IsError());
							TestFalse("FArrayReader::IsError()", ArrayReader->IsError());
							TestEqual("FSQPQueryResponseHeader::Header::Type", Output.Header.Type, Input.Header.Type);
							TestEqual("FSQPQueryResponseHeader::Header::ChallengeToken", Output.Header.ChallengeToken, Input.Header.ChallengeToken);
							TestEqual("FSQPQueryResponseHeader::Version", Output.Version, Input.Version);
							TestEqual("FSQPQueryResponseHeader::CurrentPacket", Output.CurrentPacket, Input.CurrentPacket);
							TestEqual("FSQPQueryResponseHeader::LastPacket", Output.LastPacket, Input.LastPacket);
							TestEqual("FSQPQueryResponseHeader::PacketLength", Output.PacketLength, Input.PacketLength);
						});
				});
		});

	Describe("FSQPServerInfoData", [this]()
		{
			Describe("operator<<", [this]()
				{
					It("should properly serialize and deserialize its member variables.", [this]()
						{
							TSharedPtr<FArrayWriter> ArrayWriter = MakeShared<FArrayWriter>(false);
							TSharedPtr<FArrayReader> ArrayReader = MakeShared<FArrayReader>(false);

							RocketScienceMultiplaySDK::FSQPServerInfoData Input = {};
							Input.CurrentPlayers = 0xdead;
							Input.MaxPlayers = 0xbeef;
							Input.ServerName = FString("servername01"); 
							Input.GameType = FString("gametype01"); 
							Input.BuildId = FString("buildid01");
							Input.Map = FString("map01");
							Input.Port = 0x1f90;

							RocketScienceMultiplaySDK::FSQPServerInfoData Output = {};

							*ArrayWriter << Input;
							ArrayReader->Append(ArrayWriter->GetData(), ArrayWriter->Num());
							*ArrayReader << Output;

							TestFalse("FArrayWriter::IsError()", ArrayWriter->IsError());
							TestFalse("FArrayReader::IsError()", ArrayReader->IsError());
							TestEqual("FSQPServerInfoData::CurrentPlayers", Output.CurrentPlayers, Input.CurrentPlayers);
							TestEqual("FSQPServerInfoData::MaxPlayers", Output.MaxPlayers, Input.MaxPlayers);
							TestEqual("FSQPServerInfoData::ServerName", Output.ServerName, Input.ServerName);
							TestEqual("FSQPServerInfoData::GameType", Output.GameType, Input.GameType);
							TestEqual("FSQPServerInfoData::BuildId", Output.BuildId, Input.BuildId); 
							TestEqual("FSQPServerInfoData::Map", Output.Map, Input.Map);
							TestEqual("FSQPServerInfoData::Port", Output.Port, Input.Port);
						});
				});
		});

	Describe("FSQPQueryResponsePacket", [this]()
		{
			Describe("operator<<", [this]()
				{
					It("should properly serialize and deserialize its member variables.", [this]()
						{
							TSharedPtr<FArrayWriter> ArrayWriter = MakeShared<FArrayWriter>(false);
							TSharedPtr<FArrayReader> ArrayReader = MakeShared<FArrayReader>(false);

							RocketScienceMultiplaySDK::FSQPHeader InputHeader = {};
							InputHeader.Type = static_cast<uint8>(RocketScienceMultiplaySDK::ESQPMessageType::QueryRequest);
							InputHeader.ChallengeToken = 0xdeadbeef;

							RocketScienceMultiplaySDK::FSQPQueryResponseHeader InputQueryHeader = {};
							InputQueryHeader.Header = InputHeader;
							InputQueryHeader.Version = 0xdead;
							InputQueryHeader.CurrentPacket = 0x01;
							InputQueryHeader.LastPacket = 0x99;
							// InputQueryHeader.PacketLength = *calculated by operator<<*;

							RocketScienceMultiplaySDK::FSQPServerInfoData InputServerInfoData = {};
							InputServerInfoData.CurrentPlayers = 0xdead;
							InputServerInfoData.MaxPlayers = 0xbeef;
							InputServerInfoData.ServerName = FString(TEXT("servername01"));
							InputServerInfoData.GameType = FString(TEXT("gametype01"));
							InputServerInfoData.BuildId = FString(TEXT("buildid01"));
							InputServerInfoData.Map = FString(TEXT("map01"));
							InputServerInfoData.Port = 0x8080;

							RocketScienceMultiplaySDK::FSQPQueryResponsePacket Input = {};
							Input.RequestedChunks = static_cast<uint8>(RocketScienceMultiplaySDK::ESQPChunkType::ServerInfo);
							Input.QueryHeader = InputQueryHeader;
							Input.ServerInfoData = InputServerInfoData;
							//Input.ServerInfoChunkLength = *calculated by operator<<*;

							RocketScienceMultiplaySDK::FSQPQueryResponsePacket Output = {};
							Output.RequestedChunks = Input.RequestedChunks; // Explicitly assigned because this field is not serialized and affects serialization logic.

							*ArrayWriter << Input;
							ArrayReader->Append(ArrayWriter->GetData(), ArrayWriter->Num());
							*ArrayReader << Output;

							TestFalse("FArrayWriter::IsError()", ArrayWriter->IsError());
							TestFalse("FArrayReader::IsError()", ArrayReader->IsError());

							TestEqual("FSQPQueryResponsePacket::ServerInfoChunkLength", Output.ServerInfoChunkLength, Input.ServerInfoChunkLength);

							TestEqual("FSQPQueryResponsePacket::QueryHeader::Header::Type", Output.QueryHeader.Header.Type, Input.QueryHeader.Header.Type);
							TestEqual("FSQPQueryResponsePacket::QueryHeader::Header::ChallengeToken", Output.QueryHeader.Header.ChallengeToken, Input.QueryHeader.Header.ChallengeToken);

							TestEqual("FSQPQueryResponsePacket::QueryHeader::Version", Output.QueryHeader.Version, Input.QueryHeader.Version);
							TestEqual("FSQPQueryResponsePacket::QueryHeader::CurrentPacket", Output.QueryHeader.CurrentPacket, Input.QueryHeader.CurrentPacket);
							TestEqual("FSQPQueryResponsePacket::QueryHeader::LastPacket", Output.QueryHeader.LastPacket, Input.QueryHeader.LastPacket);
							TestEqual("FSQPQueryResponsePacket::QueryHeader::PacketLength", Output.QueryHeader.PacketLength, Input.QueryHeader.PacketLength);

							TestEqual("FSQPQueryResponsePacket::ServerInfoData::CurrentPlayers", Output.ServerInfoData.CurrentPlayers, Input.ServerInfoData.CurrentPlayers);
							TestEqual("FSQPQueryResponsePacket::ServerInfoData::MaxPlayers", Output.ServerInfoData.MaxPlayers, Input.ServerInfoData.MaxPlayers);
							TestEqual("FSQPQueryResponsePacket::ServerInfoData::ServerName", Output.ServerInfoData.ServerName, Input.ServerInfoData.ServerName);
							TestEqual("FSQPQueryResponsePacket::ServerInfoData::GameType", Output.ServerInfoData.GameType, Input.ServerInfoData.GameType);
							TestEqual("FSQPQueryResponsePacket::ServerInfoData::BuildId", Output.ServerInfoData.BuildId, Input.ServerInfoData.BuildId);
							TestEqual("FSQPQueryResponsePacket::ServerInfoData::Map", Output.ServerInfoData.Map, Input.ServerInfoData.Map);
							TestEqual("FSQPQueryResponsePacket::ServerInfoData::Port", Output.ServerInfoData.Port, Input.ServerInfoData.Port);
						});
				});
		});
}

#endif // #if WITH_AUTOMATION_TESTS
