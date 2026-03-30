/*
 * #####################################################################################
 *  Rocket Science Multiplay Plugin for Unreal Engine Copyright © 2026 Rocket Science Group
 * #####################################################################################
 */

#include "Tests/AutomationCommon.h"
#include "Utils/AutomationTestUtils.h"
#include "RSMultiplayServerQueryHandlerSubsystem.h"

#if WITH_AUTOMATION_TESTS
BEGIN_DEFINE_SPEC(FRSMultiplayServerQueryHandlerSubsystemSpec, "MultiplayGameServerSDK.URSMultiplayServerQueryHandlerSubsystem", EAutomationTestFlags::ProductFilter | EAutomationTestFlags::ApplicationContextMask)
URSMultiplayServerQueryHandlerSubsystem* ServerQueryHandlerSubsystem;
END_DEFINE_SPEC(FRSMultiplayServerQueryHandlerSubsystemSpec)

void FRSMultiplayServerQueryHandlerSubsystemSpec::Define()
{
	BeforeEach([this]()
		{
			AutomationOpenMap(TEXT("/UnityGamingServicesSDK/Test"));

			UWorld* GameWorld = RocketScienceMultiplaySDK::GetAnyGameWorld();
			UGameInstance* GameInstance = GameWorld->GetGameInstance();
			ServerQueryHandlerSubsystem = GameInstance->GetSubsystem<URSMultiplayServerQueryHandlerSubsystem>();
		});

	AfterEach([this]()
		{
			// TODO: Exit PIE mode after all tests have completed running.

			ServerQueryHandlerSubsystem = nullptr;
		});

	Describe("IsConnected", [this]()
		{
			It("should return false before connecting.", [this]()
				{
					TestFalseExpr(ServerQueryHandlerSubsystem->IsConnected());
				});

			It("should return true after connecting.", [this]()
				{
					ServerQueryHandlerSubsystem->Connect();

					TestTrueExpr(ServerQueryHandlerSubsystem->IsConnected());
				});

			It("should return false after disconnecting.", [this]()
				{
					ServerQueryHandlerSubsystem->Connect();
					ServerQueryHandlerSubsystem->Disconnect();

					TestFalseExpr(ServerQueryHandlerSubsystem->IsConnected());
				});

			AfterEach([this]()
				{
					ServerQueryHandlerSubsystem->Disconnect();
				});
		});

	Describe("Connect", [this]()
		{
			It("should return true after connecting successfully.", [this]()
				{
					TestTrueExpr(ServerQueryHandlerSubsystem->Connect());
				});

			It("should return true when a connection has already been established.", [this]()
				{
					bool bDidConnectOnAttemptOne = ServerQueryHandlerSubsystem->Connect();
					bool bDidConectOnAttemptTwo = ServerQueryHandlerSubsystem->Connect();

					TestTrueExpr(bDidConnectOnAttemptOne);
					TestTrueExpr(bDidConectOnAttemptTwo);
				});

			AfterEach([this]()
				{
					ServerQueryHandlerSubsystem->Disconnect();
				});
		});

	Describe("IncrementCurrentPlayers", [this]()
		{
			AfterEach([this]()
				{
					ServerQueryHandlerSubsystem->SetCurrentPlayers(0);
					ServerQueryHandlerSubsystem->SetMaxPlayers(TNumericLimits<uint16>::Max());
				});

			It("should increment CurrentPlayers by one.", [this]()
				{
					int32 Before = ServerQueryHandlerSubsystem->GetCurrentPlayers();

					ServerQueryHandlerSubsystem->IncrementCurrentPlayers();

					int32 After = ServerQueryHandlerSubsystem->GetCurrentPlayers();

					TestEqual(TEXT("CurrentPlayers was not incremented by one"), After, (Before + 1));
				});

			It("should not increment CurrentPlayers above UINT16_MAX.", [this]()
				{
					AddExpectedError(TEXT("Cannot increment CurrentPlayers above the maximum number of players, \\d+\\."), EAutomationExpectedErrorFlags::Exact, 1);

					int32 MaximumNumberOfPlayers = TNumericLimits<uint16>::Max();
					ServerQueryHandlerSubsystem->SetMaxPlayers(MaximumNumberOfPlayers);
					ServerQueryHandlerSubsystem->SetCurrentPlayers(MaximumNumberOfPlayers);

					int32 Before = ServerQueryHandlerSubsystem->GetCurrentPlayers();

					ServerQueryHandlerSubsystem->IncrementCurrentPlayers();

					int32 After = ServerQueryHandlerSubsystem->GetCurrentPlayers();

					TestEqual(TEXT("CurrentPlayers was incremented above UNIT16_MAX"), After, Before);
				});

			It("should not increment CurrentPlayers above MaxPlayers", [this]()
				{
					AddExpectedError(TEXT("Cannot increment CurrentPlayers above the maximum number of players, \\d+\\."), EAutomationExpectedErrorFlags::Exact, 1);

					int32 MaximumNumberOfPlayers = 4;
					ServerQueryHandlerSubsystem->SetMaxPlayers(MaximumNumberOfPlayers);
					ServerQueryHandlerSubsystem->SetCurrentPlayers(MaximumNumberOfPlayers);

					int32 Before = ServerQueryHandlerSubsystem->GetCurrentPlayers();

					ServerQueryHandlerSubsystem->IncrementCurrentPlayers();

					int32 After = ServerQueryHandlerSubsystem->GetCurrentPlayers();

					TestEqual(TEXT("CurrentPlayers was incremented above MaxPlayers"), After, Before);
				});
		});

	Describe("DecrementCurrentPlayers", [this]()
		{
			It("should decrement CurrentPlayers by one.", [this]()
				{
					ServerQueryHandlerSubsystem->SetCurrentPlayers(1);

					int32 Before = ServerQueryHandlerSubsystem->GetCurrentPlayers();

					ServerQueryHandlerSubsystem->DecrementCurrentPlayers();

					int32 After = ServerQueryHandlerSubsystem->GetCurrentPlayers();

					TestEqual(TEXT("CurrentPlayers was not decremented by one"), After, (Before - 1));
				});

			It("should not decrement CurrentPlayers below UINT16_MIN.", [this]()
				{
					AddExpectedError(TEXT("Cannot decrement CurrentPlayers below UINT16_MIN."), EAutomationExpectedErrorFlags::Exact, 1);

					int32 MinimumNumberOfPlayers = TNumericLimits<uint16>::Min();
					ServerQueryHandlerSubsystem->SetCurrentPlayers(MinimumNumberOfPlayers);

					int32 Before = ServerQueryHandlerSubsystem->GetCurrentPlayers();

					ServerQueryHandlerSubsystem->DecrementCurrentPlayers();

					int32 After = ServerQueryHandlerSubsystem->GetCurrentPlayers();

					TestEqual(TEXT("CurrentPlayers was decremented below UNIT16_MIN"), After, Before);
				});
		});

	Describe("SetCurrentPlayers", [this]()
		{
			AfterEach([this]()
				{
					ServerQueryHandlerSubsystem->SetCurrentPlayers(0);
					ServerQueryHandlerSubsystem->SetMaxPlayers(TNumericLimits<uint16>::Max());
				});

			It("should update CurrentPlayers when given valid data.", [this]()
				{
					int32 CurrentPlayers = 4;

					ServerQueryHandlerSubsystem->SetCurrentPlayers(CurrentPlayers);

					TestEqual("CurrentPlayers does not contain expected value", ServerQueryHandlerSubsystem->GetCurrentPlayers(), CurrentPlayers);
				});

			It("should not update CurrentPlayers when value is too large to represent using a uint16.", [this]()
				{
					AddExpectedError(TEXT("Attempted to assign a value greater than the maximum number of allowed players, \\d+, it will be ignored\\."), EAutomationExpectedErrorFlags::MatchType::Exact, 1);

					int32 CurrentPlayers = TNumericLimits<uint16>::Max() + 1;

					ServerQueryHandlerSubsystem->SetCurrentPlayers(CurrentPlayers);

					TestNotEqual("CurrentPlayers contains unexpected value", ServerQueryHandlerSubsystem->GetCurrentPlayers(), CurrentPlayers);
				});

			It("should not update CurrentPlayers when value is too small to represent using a uint16.", [this]()
				{
					AddExpectedError(TEXT("Attempted to assign a value that cannot be represented by a uint16 to CurrentPlayers, it will be ignored"), EAutomationExpectedErrorFlags::MatchType::Exact, 1);

					int32 CurrentPlayers = TNumericLimits<uint16>::Min() - 1;

					ServerQueryHandlerSubsystem->SetCurrentPlayers(CurrentPlayers);

					TestNotEqual("CurrentPlayers contains unexpected value", ServerQueryHandlerSubsystem->GetCurrentPlayers(), CurrentPlayers);
				});

			It("should not update CurrentPlayers when value is greater than MaxPlayers.", [this]()
				{
					AddExpectedError(TEXT("Attempted to assign a value greater than the maximum number of allowed players, \\d+, it will be ignored\\."), EAutomationExpectedErrorFlags::MatchType::Exact, 1);

					int32 MaximumNumberOfPlayers = 4;
					ServerQueryHandlerSubsystem->SetMaxPlayers(MaximumNumberOfPlayers);

					int32 Before = ServerQueryHandlerSubsystem->GetCurrentPlayers();

					ServerQueryHandlerSubsystem->SetCurrentPlayers(MaximumNumberOfPlayers + 1);

					int32 After = ServerQueryHandlerSubsystem->GetCurrentPlayers();

					TestEqual(TEXT("CurrentPlayers contains unexpected value"), After, Before);
				});
		});

	Describe("SetMaxPlayers", [this]()
		{
			It("should update MaxPlayers when given valid data.", [this]()
				{
					int32 MaxPlayers = 8;

					ServerQueryHandlerSubsystem->SetMaxPlayers(MaxPlayers);

					TestEqual("MaxPlayers does not contain expected value", ServerQueryHandlerSubsystem->GetMaxPlayers(), MaxPlayers);
				});

			It("should not update MaxPlayers when value is too large to represent using a uint16.", [this]()
				{
					AddExpectedError(TEXT("Attempted to assign a value that cannot be represented by a uint16 to MaxPlayers, it will be ignored"), EAutomationExpectedErrorFlags::MatchType::Exact, 1);

					int32 MaxPlayers = TNumericLimits<uint16>::Max() + 1;

					ServerQueryHandlerSubsystem->SetMaxPlayers(MaxPlayers);

					TestNotEqual("MaxPlayers contains unexpected value", ServerQueryHandlerSubsystem->GetMaxPlayers(), MaxPlayers);
				});

			It("should not update MaxPlayers when value is too small to represent using a uint16.", [this]()
				{
					AddExpectedError(TEXT("Attempted to assign a value that cannot be represented by a uint16 to MaxPlayers, it will be ignored"), EAutomationExpectedErrorFlags::MatchType::Exact, 1);

					int32 MaxPlayers = TNumericLimits<uint16>::Min() - 1;

					ServerQueryHandlerSubsystem->SetMaxPlayers(MaxPlayers);

					TestNotEqual("MaxPlayers contains unexpected value", ServerQueryHandlerSubsystem->GetMaxPlayers(), MaxPlayers);
				});
		});

	Describe("SetServerName", [this]()
		{
			It("should update ServerName when given valid data.", [this]()
				{
					FString ServerName = TEXT("Unreal Engine 4 Dedicated Server");

					ServerQueryHandlerSubsystem->SetServerName(ServerName);

					TestEqual("ServerName does not contain expected value", ServerQueryHandlerSubsystem->GetServerName(), ServerName);
				});

			It("should not update ServerName when assigned a string exceeding 255 characters in length.", [this]()
				{
					AddExpectedError(TEXT("Attempted to assign a string longer than 255 characters to ServerName, which is unsupported by the server query protocol, this value will be ignored"), EAutomationExpectedErrorFlags::MatchType::Exact, 1);

					FString ServerName = TEXT("Lorem ipsum dolor sit amet, consectetuer adipiscing elit. Aenean commodo ligula eget dolor. Aenean massa. Cum sociis natoque penatibus et magnis dis parturient montes, nascetur ridiculus mus. Donec quam felis, ultricies nec, pellentesque eu, pretium quis,.");

					ServerQueryHandlerSubsystem->SetServerName(ServerName);

					TestNotEqual("ServerName contains unexpected value", ServerQueryHandlerSubsystem->GetServerName(), ServerName);
				});
		});

	Describe("SetGameType", [this]()
		{
			It("should update GameType when given valid data.", [this]()
				{
					FString GameType = TEXT("Capture the Flag");

					ServerQueryHandlerSubsystem->SetGameType(GameType);

					TestEqual("GameType does not contain expected value", ServerQueryHandlerSubsystem->GetGameType(), GameType);
				});

			It("should not update GameType when assigned a string exceeding 255 characters in length.", [this]()
				{
					AddExpectedError(TEXT("Attempted to assign a string longer than 255 characters to GameType, which is unsupported by the server query protocol, this value will be ignored"), EAutomationExpectedErrorFlags::MatchType::Exact, 1);

					FString GameType = TEXT("Lorem ipsum dolor sit amet, consectetuer adipiscing elit. Aenean commodo ligula eget dolor. Aenean massa. Cum sociis natoque penatibus et magnis dis parturient montes, nascetur ridiculus mus. Donec quam felis, ultricies nec, pellentesque eu, pretium quis,.");

					ServerQueryHandlerSubsystem->SetGameType(GameType);

					TestNotEqual("GameType contains unexpected value", ServerQueryHandlerSubsystem->GetGameType(), GameType);
				});
		});

	Describe("SetBuildId", [this]()
		{
			It("should update BuildId when given valid data.", [this]()
				{
					FString BuildId = TEXT("v1.0.0");

					ServerQueryHandlerSubsystem->SetBuildId(BuildId);

					TestEqual("BuildId does not contain expected value", ServerQueryHandlerSubsystem->GetBuildId(), BuildId);
				});

			It("should not update BuildId when assigned a string exceeding 255 characters in length.", [this]()
				{
					AddExpectedError(TEXT("Attempted to assign a string longer than 255 characters to BuildId, which is unsupported by the server query protocol, this value will be ignored"), EAutomationExpectedErrorFlags::MatchType::Exact, 1);

					FString BuildId = TEXT("Lorem ipsum dolor sit amet, consectetuer adipiscing elit. Aenean commodo ligula eget dolor. Aenean massa. Cum sociis natoque penatibus et magnis dis parturient montes, nascetur ridiculus mus. Donec quam felis, ultricies nec, pellentesque eu, pretium quis,.");

					ServerQueryHandlerSubsystem->SetBuildId(BuildId);

					TestNotEqual("BuildId contains unexpected value", ServerQueryHandlerSubsystem->GetBuildId(), BuildId);
				});
		});

	Describe("SetMap", [this]()
		{
			It("should update Map when given valid data.", [this]()
				{
					FString Map = TEXT("CTF-Face");

					ServerQueryHandlerSubsystem->SetMap(Map);

					TestEqual("Map does not contain expected value", ServerQueryHandlerSubsystem->GetMap(), Map);
				});

			It("should not update Map when assigned a string exceeding 255 characters in length.", [this]()
				{
					AddExpectedError(TEXT("Attempted to assign a string longer than 255 characters to Map, which is unsupported by the server query protocol, this value will be ignored"), EAutomationExpectedErrorFlags::MatchType::Exact, 1);

					FString Map = TEXT("Lorem ipsum dolor sit amet, consectetuer adipiscing elit. Aenean commodo ligula eget dolor. Aenean massa. Cum sociis natoque penatibus et magnis dis parturient montes, nascetur ridiculus mus. Donec quam felis, ultricies nec, pellentesque eu, pretium quis,.");

					ServerQueryHandlerSubsystem->SetMap(Map);

					TestNotEqual("Map contains unexpected value", ServerQueryHandlerSubsystem->GetMap(), Map);
				});
		});

	Describe("SetPort", [this]()
		{
			It("should update Port when given valid data.", [this]()
				{
					int32 Port = 7777;

					ServerQueryHandlerSubsystem->SetPort(Port);

					TestEqual("Port does not contain expected value", ServerQueryHandlerSubsystem->GetPort(), Port);
				});

			It("should not update Port when value is too large to represent using a uint16.", [this]()
				{
					AddExpectedError(TEXT("Attempted to assign a value that cannot be represented by a uint16 to Port, it will be ignored"), EAutomationExpectedErrorFlags::MatchType::Exact, 1);

					int32 Port = TNumericLimits<uint16>::Max() + 1;

					ServerQueryHandlerSubsystem->SetPort(Port);

					TestNotEqual("Port contains unexpected value", ServerQueryHandlerSubsystem->GetPort(), Port);
				});

			It("should not update Port when value is too small to represent using a uint16.", [this]()
				{
					AddExpectedError(TEXT("Attempted to assign a value that cannot be represented by a uint16 to Port, it will be ignored"), EAutomationExpectedErrorFlags::MatchType::Exact, 1);

					int32 Port = TNumericLimits<uint16>::Min() - 1;

					ServerQueryHandlerSubsystem->SetPort(Port);

					TestNotEqual("Port contains unexpected value", ServerQueryHandlerSubsystem->GetPort(), Port);
				});
		});
}

#endif // #if WITH_AUTOMATION_TESTS
