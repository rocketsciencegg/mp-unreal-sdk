/*
 * #####################################################################################
 *  Rocket Science Multiplay Plugin for Unreal Engine Copyright © 2026 Rocket Science Group
 * #####################################################################################
 */

#include "Tests/AutomationCommon.h"
#include "MultiplayGameServerSDK/MultiplayServerEvents.h"

#if WITH_AUTOMATION_TESTS
BEGIN_DEFINE_SPEC(FRSMultiplayServerEventsSpec, "MultiplayGameServerSDK.ServerEvents", 	
	EAutomationTestFlags::ProductFilter | 
	EAutomationTestFlags::EditorContext | 
	EAutomationTestFlags::ClientContext | 
	EAutomationTestFlags::ServerContext | 
	EAutomationTestFlags::CommandletContext)
END_DEFINE_SPEC(FRSMultiplayServerEventsSpec)

void FRSMultiplayServerEventsSpec::Define()
{
	Describe("FMultiplayServerAllocateEvent", [this]()
		{
			Describe("FromJson", [this]()
				{
					It("should return true and populate the members of FMultiplayServerAllocateEvent when parsing valid JSON.", [this]()
						{
							FString JsonString = TEXT("{\"EventID\":\"e3e455f8-f977-11e9-bccf-1a111111f111\",\"EventType\":\"AllocateEventType\",\"ServerID\":12345,\"AllocationID\":\"e3e455f8-f977-11e9-bccf-2a222222f222\"}");

							TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<>::Create(JsonString);

							TSharedPtr<FJsonValue> JsonValue;
							FJsonSerializer::Deserialize(JsonReader, JsonValue);

							RocketScienceMultiplaySDK::FMultiplayServerAllocateEvent ServerAllocateEvent;
							bool bDidParseJson = ServerAllocateEvent.FromJson(JsonValue);

							TestTrue("bDidParseJson is true", bDidParseJson);

							FGuid eventIdGuid;
							FGuid::Parse("e3e455f8-f977-11e9-bccf-1a111111f111", eventIdGuid);
							TestEqual("FMultiplayServerAllocateEvent::EventId is populated", ServerAllocateEvent.EventId, eventIdGuid);
							TestEqual("FMultiplayServerAllocateEvent::EventType is populated", ServerAllocateEvent.EventType, RocketScienceMultiplaySDK::FMultiplayServerAllocateEvent::kEventType);
							TestEqual("FMultiplayServerAllocateEvent::ServerId is populated", ServerAllocateEvent.ServerId, 12345);

							FGuid allocationIdGuid;
							FGuid::Parse("e3e455f8-f977-11e9-bccf-2a222222f222", allocationIdGuid);
							TestEqual("FMultiplayServerAllocateEvent::AllocationId is populated", ServerAllocateEvent.AllocationId, allocationIdGuid);
						});
				});
		});

	Describe("FMultiplayServerDeallocateEvent", [this]()
		{
			Describe("FromJson", [this]()
				{
					It("should return true and populate the members of FMultiplayServerDeallocateEvent when parsing valid JSON.", [this]()
						{
							FString JsonString = TEXT("{\"EventID\":\"e3e455f8-f977-11e9-bccf-1a111111f111\",\"EventType\":\"DeallocateEventType\",\"ServerID\":12345,\"AllocationID\":\"e3e455f8-f977-11e9-bccf-2a222222f222\"}");

							TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<>::Create(JsonString);

							TSharedPtr<FJsonValue> JsonValue;
							FJsonSerializer::Deserialize(JsonReader, JsonValue);

							RocketScienceMultiplaySDK::FMultiplayServerDeallocateEvent ServerDeallocateEvent;
							bool bDidParseJson = ServerDeallocateEvent.FromJson(JsonValue);

							TestTrue("bDidParseJson is true", bDidParseJson);
							
							FGuid eventIdGuid;
							FGuid::Parse("e3e455f8-f977-11e9-bccf-1a111111f111", eventIdGuid);
							TestEqual("FMultiplayServerDeallocateEvent::EventId is populated", ServerDeallocateEvent.EventId, eventIdGuid);
							TestEqual("FMultiplayServerDeallocateEvent::EventType is populated", ServerDeallocateEvent.EventType, RocketScienceMultiplaySDK::FMultiplayServerDeallocateEvent::kEventType);
							TestEqual("FMultiplayServerDeallocateEvent::ServerId is populated", ServerDeallocateEvent.ServerId, 12345);

							FGuid allocationIdGuid;
							FGuid::Parse("e3e455f8-f977-11e9-bccf-2a222222f222", allocationIdGuid);
							TestEqual("FMultiplayServerDeallocateEvent::AllocationId is populated", ServerDeallocateEvent.AllocationId, allocationIdGuid);
						});
				});
		});
}

#endif // #if WITH_AUTOMATION_TESTS
