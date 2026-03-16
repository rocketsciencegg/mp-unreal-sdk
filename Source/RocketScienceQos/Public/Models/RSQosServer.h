/*
 * #####################################################################################
 *  Unity Gaming Services Plugin for Unreal Engine Copyright © 2025 Unity Technologies.
 * #####################################################################################
 */

#pragma once

#include "CoreMinimal.h"
#include "RSQosServer.generated.h"

 /**
  * The Multiplay allocation for the server.
  */
USTRUCT(BlueprintType)
struct ROCKETSCIENCEQOS_API FRSQosServer
{
    GENERATED_BODY()

    /**
    * Endpoints at which you can reach the QoS server.
    */
    UPROPERTY(BlueprintReadOnly, Category = "Unity Gaming Services | Qos | Server")
    TArray<FString> Endpoints;

    /**
    * IPv4 address of the QoS server.
    */
    UPROPERTY(BlueprintReadOnly, Category = "Unity Gaming Services | Qos | Server")
    FString Ip;

    /**
    * Port of the QoS server.
    */
    UPROPERTY(BlueprintReadOnly, Category = "Unity Gaming Services | Qos | Server")
    FString Port;

    /**
    * The region to which the QoS server belongs.
    */
    UPROPERTY(BlueprintReadOnly, Category = "Unity Gaming Services | Qos | Server")
    FString Region;
};
