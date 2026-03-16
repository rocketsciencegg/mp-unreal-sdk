/*
 * #####################################################################################
 *  Rocket Science Multiplay Plugin for Unreal Engine Copyright © 2026 Rocket Science Group
 * #####################################################################################
 */
 
#include "RSMultiplaySubsystemBase.h"
#include "MultiplayGameServerSDKLog.h"
#include "Misc/CommandLine.h"

bool URSMultiplaySubsystemBase::ShouldCreateSubsystem(UObject* Outer) const
{
	UE_LOG(LogRSMultiplayGameServerSDK, Verbose, TEXT("URSMultiplaySubsystemBase::ShouldCreateSubsystem() (Called by %s)"), *(this->GetClass()->GetName()));

	TArray<FString> Tokens, Switches;
	FCommandLine::Parse(FCommandLine::Get(), Tokens, Switches);
	bool HasLocalFlag = Switches.Contains(LocalServerFlag);
	if (HasLocalFlag)
	{
		UE_LOG(LogRSMultiplayGameServerSDK, Log, TEXT("Multiplay Subsystem '%s' disabled by '%s' flag"), *(this->GetClass()->GetName()), *LocalServerFlag);
	}

	return !HasLocalFlag;
}