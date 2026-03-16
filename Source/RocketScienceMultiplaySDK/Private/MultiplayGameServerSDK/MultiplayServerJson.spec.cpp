/*
 * #####################################################################################
 *  Rocket Science Multiplay Plugin for Unreal Engine Copyright © 2026 Rocket Science Group
 * #####################################################################################
 */

#include "Tests/AutomationCommon.h"
#include "MultiplayGameServerSDK/MultiplayServerJson.h"

#if WITH_AUTOMATION_TESTS

#if ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION >= 5
BEGIN_DEFINE_SPEC(FRSMultiplayServerJsonSpec, "MultiplayGameServerSDK.FMultiplayServerJson", EAutomationTestFlags::ProductFilter | EAutomationTestFlags_ApplicationContextMask)
#else
BEGIN_DEFINE_SPEC(FMultiplayServerJsonSpec, "MultiplayGameServerSDK.FMultiplayServerJson", EAutomationTestFlags::ProductFilter | EAutomationTestFlags::ApplicationContextMask)
#endif
END_DEFINE_SPEC(FRSMultiplayServerJsonSpec)

void FRSMultiplayServerJsonSpec::Define()
{
	Describe("FromJson", [this]()
		{
			It("should return true when serverID, queryPort, and port are integers.", [this]()
				{
					int64 ServerId = 12345;
					FString AllocationId = TEXT("AAAAAAAA-BBBB-CCCC-DDDDD-EEEEEEEEEEEE");
					uint16 QueryPort = 7778;
					FString IpAddress = TEXT("0.0.0.0");
					uint16 Port = 7777;
					FString ServerLogDirectory = TEXT("/home");

					FString JsonString = FString::Printf(TEXT("{\"serverID\": %d, \"allocatedUUID\": \"%s\", \"queryPort\": %d, \"ip\": \"%s\", \"port\": %d, \"serverLogDir\": \"%s\"}"), ServerId, *AllocationId, QueryPort, *IpAddress, Port, *ServerLogDirectory);

					TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<>::Create(JsonString);

					TSharedPtr<FJsonValue> JsonValue;
					FJsonSerializer::Deserialize(JsonReader, JsonValue);

					RocketScienceMultiplaySDK::FMultiplayServerJson ServerJson;
					bool bDidParseJson = ServerJson.FromJson(JsonValue);

					TestTrue(TEXT("bDidParseJson is true"), bDidParseJson);
					TestEqual(TEXT("FMultiplayServerJson::ServerId has the correct value"), ServerJson.ServerId, ServerId);
					TestEqual(TEXT("FMultiplayServerJson::AllocationId has the correct value"), ServerJson.AllocationId, AllocationId);
					TestEqual(TEXT("FMultiplayServerJson::QueryPort has the correct value"), ServerJson.QueryPort, QueryPort);
					TestEqual(TEXT("FMultiplayServerJson::Ip has the correct value"), ServerJson.Ip, IpAddress);
					TestEqual(TEXT("FMultiplayServerJson::Port has the correct value"), ServerJson.Port, Port);
					TestEqual(TEXT("FMultiplayServerJson::ServerLogDirectory has the correct value"), ServerJson.ServerLogDirectory, ServerLogDirectory);
				});

			It("should return true when serverID, queryPort, ip, and port are strings.", [this]()
				{
					int64 ServerId = 12345;
					FString AllocationId = TEXT("AAAAAAAA-BBBB-CCCC-DDDDD-EEEEEEEEEEEE");
					uint16 QueryPort = 7778;
					FString IpAddress = TEXT("0.0.0.0");
					uint16 Port = 7777;
					FString ServerLogDirectory = TEXT("/home");

					FString JsonString = FString::Printf(TEXT("{\"serverID\": %d, \"allocatedUUID\": \"%s\", \"queryPort\": %d, \"ip\": \"%s\", \"port\": %d, \"serverLogDir\": \"%s\"}"), ServerId, *AllocationId, QueryPort, *IpAddress, Port, *ServerLogDirectory);

					TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<>::Create(JsonString);

					TSharedPtr<FJsonValue> JsonValue;
					FJsonSerializer::Deserialize(JsonReader, JsonValue);

					RocketScienceMultiplaySDK::FMultiplayServerJson ServerJson;
					bool bDidParseJson = ServerJson.FromJson(JsonValue);

					TestTrue(TEXT("bDidParseJson is true"), bDidParseJson);
					TestEqual(TEXT("FMultiplayServerJson::ServerId has the correct value"), ServerJson.ServerId, ServerId);
					TestEqual(TEXT("FMultiplayServerJson::AllocationId has the correct value"), ServerJson.AllocationId, AllocationId);
					TestEqual(TEXT("FMultiplayServerJson::QueryPort has the correct value"), ServerJson.QueryPort, QueryPort);
					TestEqual(TEXT("FMultiplayServerJson::Ip has the correct value"), ServerJson.Ip, IpAddress);
					TestEqual(TEXT("FMultiplayServerJson::Port has the correct value"), ServerJson.Port, Port);
					TestEqual(TEXT("FMultiplayServerJson::ServerLogDirectory has the correct value"), ServerJson.ServerLogDirectory, ServerLogDirectory);
				});
		});
}

#endif // #if WITH_AUTOMATION_TESTS
