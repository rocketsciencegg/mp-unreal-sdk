/*
 * #####################################################################################
 *  Rocket Science Multiplay Plugin for Unreal Engine Copyright © 2026 Rocket Science Group
 * #####################################################################################
 */

#include "RocketScienceMultiplaySDKModule.h"

void FRocketScienceMultiplaySDKModule::StartupModule()
{
	FModuleManager::Get().LoadModuleChecked("Core");
	FModuleManager::Get().LoadModuleChecked("HTTP");
	FModuleManager::Get().LoadModuleChecked("Json");
	FModuleManager::Get().LoadModuleChecked("JsonUtilities");
	FModuleManager::Get().LoadModuleChecked("Networking");
	FModuleManager::Get().LoadModuleChecked("Sockets");
	FModuleManager::Get().LoadModuleChecked("WebSockets");
}

void FRocketScienceMultiplaySDKModule::ShutdownModule()
{
}

IMPLEMENT_MODULE(FRocketScienceMultiplaySDKModule, RocketScienceMultiplaySDK)