/*
 * #####################################################################################
 *  Rocket Science Multiplay Plugin for Unreal Engine Copyright © 2026 Rocket Science Group
 * #####################################################################################
 */

#pragma once

#include "CoreMinimal.h"
#include "RSMultiplaySubsystemBase.h"
#include "RSMultiplayServerConfig.h"
#include "RSMultiplayServerConfigSubsystem.generated.h"

/**
 * Subsystem responsible for retrieving the Multiplay server configuration.
 */
UCLASS()
class ROCKETSCIENCEMULTIPLAYSDK_API URSMultiplayServerConfigSubsystem : public URSMultiplaySubsystemBase
{
	GENERATED_BODY()

public:
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;

    /**
     * @brief Accessor for the server configuration.
     * @return The server configuration for the current session.
     */
    const FRSMultiplayServerConfig& GetServerConfig() const;

protected:
    // Always create the config subsystem - other subsystems depend on it for port info
    virtual bool ShouldCreateSubsystem(UObject* Outer) const override { return true; }

private:
    /**
     * The server configuration for the current session.
     */
    UPROPERTY(BlueprintReadOnly, Category="Multiplay | ServerConfig", meta = (AllowPrivateAccess = "true"))
    FRSMultiplayServerConfig ServerConfig;
};
