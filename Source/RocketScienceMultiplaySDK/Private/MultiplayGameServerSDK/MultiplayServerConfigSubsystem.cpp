/*
 * #####################################################################################
 *  Rocket Science Multiplay Plugin for Unreal Engine Copyright © 2026 Rocket Science Group
 * #####################################################################################
 */

#include "RSMultiplayServerConfigSubsystem.h"
#include "Misc/Paths.h"
#include "Misc/FileHelper.h"
#include "Engine/GameInstance.h"
#include "Subsystems/SubsystemCollection.h"
#include "MultiplayServerJson.h"
#include "MultiplayGameServerSDKLog.h"

void URSMultiplayServerConfigSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);

#if PLATFORM_WINDOWS
    FString HomeDrive = FPlatformMisc::GetEnvironmentVariable(TEXT("HOMEDRIVE"));
    FString HomePath = FPlatformMisc::GetEnvironmentVariable(TEXT("HOMEPATH"));
    FString PathToHomeDirectory = FPaths::Combine(HomeDrive, HomePath);
#elif PLATFORM_LINUX
    FString PathToHomeDirectory = FPlatformMisc::GetEnvironmentVariable(TEXT("HOME"));
#else
    FString PathToHomeDirectory = TEXT("");
#endif

    FString PathToServerJson = FPaths::Combine(PathToHomeDirectory, TEXT("server.json"));

    bool bDidReadServerJson = false;
    FString ServerJsonFileContents;
    if (FFileHelper::LoadFileToString(ServerJsonFileContents, *PathToServerJson))
    {
        auto JsonReader = TJsonReaderFactory<>::Create(ServerJsonFileContents);

        TSharedPtr<FJsonValue> ServerJsonValue;
        if (FJsonSerializer::Deserialize(JsonReader, ServerJsonValue) && ServerJsonValue.IsValid())
        {
            RocketScienceMultiplaySDK::FMultiplayServerJson ServerJson;
            if (ServerJson.FromJson(ServerJsonValue))
            {
                UE_LOG(LogRSMultiplayGameServerSDK, Log, TEXT("Retrieved Server Id: %lld"), ServerJson.ServerId);
                UE_LOG(LogRSMultiplayGameServerSDK, Log, TEXT("Retrieved Allocation Id: %s"), *ServerJson.AllocationId);
                UE_LOG(LogRSMultiplayGameServerSDK, Log, TEXT("Retrieved Query Port: %d"), ServerJson.QueryPort);
                UE_LOG(LogRSMultiplayGameServerSDK, Log, TEXT("Retrieved Ip: %s"), *ServerJson.Ip);
                UE_LOG(LogRSMultiplayGameServerSDK, Log, TEXT("Retrieved Port: %d"), ServerJson.Port);
                UE_LOG(LogRSMultiplayGameServerSDK, Log, TEXT("Retrieved Server Log Directory: %s"), *ServerJson.ServerLogDirectory);

                bDidReadServerJson = true;

                ServerConfig.ServerId = ServerJson.ServerId;
                ServerConfig.AllocationId = ServerJson.AllocationId;
                ServerConfig.QueryPort = ServerJson.QueryPort;
                ServerConfig.Ip = ServerJson.Ip;
                ServerConfig.Port = ServerJson.Port;
                ServerConfig.ServerLogDirectory = ServerJson.ServerLogDirectory;
            }
        }
    }

    if (!bDidReadServerJson)
    {
        ServerConfig.ServerId = 0;
        ServerConfig.AllocationId = TEXT("");
        ServerConfig.QueryPort = 0;
        ServerConfig.Ip = TEXT("");
        ServerConfig.Port = 0;
        ServerConfig.ServerLogDirectory = TEXT("");

#if WITH_EDITOR
        UE_LOG(LogRSMultiplayGameServerSDK, Warning, TEXT("Failed to read server ID from %s, defaulting to invalid server ID %d."), *PathToServerJson, ServerConfig.ServerId);
#else
        UE_LOG(LogRSMultiplayGameServerSDK, Error, TEXT("Failed to read server ID from %s."), *PathToServerJson);
#endif

#if WITH_EDITOR
        UE_LOG(LogRSMultiplayGameServerSDK, Warning, TEXT("Failed to read query port from %s, defaulting to an ephemeral port."), *PathToServerJson);
#else
        UE_LOG(LogRSMultiplayGameServerSDK, Error, TEXT("Failed to read query port from %s."), *PathToServerJson);
#endif
    }
}

const FRSMultiplayServerConfig& URSMultiplayServerConfigSubsystem::GetServerConfig() const
{
    return ServerConfig;
}
