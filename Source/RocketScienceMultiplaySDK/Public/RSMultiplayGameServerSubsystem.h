/*
 * #####################################################################################
 *  Rocket Science Multiplay Plugin for Unreal Engine Copyright © 2026 Rocket Science Group
 * #####################################################################################
 */

#pragma once

#include "CoreMinimal.h"
#include "RSMultiplaySubsystemBase.h"
#include "RSMultiplayAllocation.h"
#include "RSMultiplayDeallocation.h"
#include "RSMultiplayErrorResponse.h"
#include "RSMultiplayPayloadAllocationErrorResponse.h"
#include "RSMultiplayPayloadTokenResponse.h"
#include "RSMultiplayGameServerSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FRSAllocateDelegate, FRSMultiplayAllocation, Allocation);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FRSDeallocateDelegate, FRSMultiplayDeallocation, Deallocation);

DECLARE_DYNAMIC_DELEGATE(FRSReadyServerSuccessDelegate);
DECLARE_DYNAMIC_DELEGATE_OneParam(FRSReadyServerFailureDelegate, FRSMultiplayErrorResponse, ErrorResponse);

DECLARE_DYNAMIC_DELEGATE(FRSUnreadyServerSuccessDelegate);
DECLARE_DYNAMIC_DELEGATE_OneParam(FRSUnreadyServerFailureDelegate, FRSMultiplayErrorResponse, ErrorResponse);

DECLARE_DYNAMIC_DELEGATE_OneParam(FRSPayloadAllocationSuccessDelegate, FString, Payload);
DECLARE_DYNAMIC_DELEGATE_OneParam(FRSPayloadAllocationFailureDelegate, FRSMultiplayPayloadAllocationErrorResponse, ErrorResponse);

DECLARE_DYNAMIC_DELEGATE_OneParam(FRSPayloadTokenSuccessDelegate, FRSMultiplayPayloadTokenResponse, TokenResponse);
DECLARE_DYNAMIC_DELEGATE_OneParam(FRSPayloadTokenFailureDelegate, FRSMultiplayPayloadTokenResponse, ErrorResponse);

namespace RocketScienceMultiplaySDK
{
	class FCentrifugeClient;
	class FConnectResult;
	class FPublication;

	class OpenAPIGameServerApi;
	class ReadyServerResponse;
	class UnreadyServerResponse;

	class OpenAPIPayloadApi;
	class PayloadAllocationResponse;
	class PayloadTokenResponse;
}

/**
  * @brief Subsystem responsible for communicating with the Multiplay SDK daemon. 
  */
UCLASS()
class ROCKETSCIENCEMULTIPLAYSDK_API URSMultiplayGameServerSubsystem : public URSMultiplaySubsystemBase
{
	GENERATED_BODY()

public:
	/**
	 * Subsystem functions, overrides from USubsystem.
	 */
	URSMultiplayGameServerSubsystem();
	virtual ~URSMultiplayGameServerSubsystem();
	URSMultiplayGameServerSubsystem(FVTableHelper& Helper);
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

public:
	// Multiplay Game Server SDK interface.

	/**
	 * @brief Marks the server as ready for players. The client invokes this method when it is ready to start accepting connections. 
	 * @param OnSuccess This delegate will be invoked if the operation completes successfully.
	 * @param OnFailure This delegate will be invoked if the operation is unsuccessful.
	 */
	UFUNCTION(BlueprintCallable, Category="Multiplay | GameServer")
	void ReadyServerForPlayers(FRSReadyServerSuccessDelegate OnSuccess, FRSReadyServerFailureDelegate OnFailure);

	/**
	 * @brief Marks the server as not ready for players. The client invokes this method when wants to stop accepting connections.
	 * @param OnSuccess This delegate will be invoked if the operation completes successfully.
	 * @param OnFailure This delegate will be invoked if the operation is unsuccessful.
	 */
	UFUNCTION(BlueprintCallable, Category="Multiplay | GameServer")
	void UnreadyServer(FRSUnreadyServerSuccessDelegate OnSuccess, FRSUnreadyServerFailureDelegate OnFailure);

	/**
	 * @brief Establishes a connection to the Multiplay SDK daemon and subscribes to allocation messages.
	 */
	UFUNCTION(BlueprintCallable, Category="Multiplay | GameServer")
	void SubscribeToServerEvents();

	/**
	 * @brief Cleans up connection to the Multiplay SDK daemon and unsubscribe from allocation messages.
	 */
	UFUNCTION(BlueprintCallable, Category="Multiplay | GameServer")
	void UnsubscribeToServerEvents();

	/**
	 * @brief Retrieves the allocation payload.
	 * @param OnSuccess This delegate will be invoked if the operation completes successfully.
	 * @param OnFailure This delegate will be invoked if the operation is unsuccessful.
	 */
	UFUNCTION(BlueprintCallable, Category="Multiplay | GameServer")
	void GetPayloadAllocation(FRSPayloadAllocationSuccessDelegate OnSuccess, FRSPayloadAllocationFailureDelegate OnFailure);

	/**
	 * @brief Retrieves a JWT token for payloads.
	 * @param OnSuccess This delegate will be invoked if the operation completes successfully.
	 * @param OnFailure This delegate will be invoked if the operation is unsuccessful.
	 */
	UFUNCTION(BlueprintCallable, Category="Multiplay | GameServer")
	void GetPayloadToken(FRSPayloadTokenSuccessDelegate OnSuccess, FRSPayloadTokenFailureDelegate OnFailure);

    /**
     * Delegate that is invoked when this server has been allocated.
     */
	UPROPERTY(BlueprintAssignable, Category="Multiplay | GameServer")
	FRSAllocateDelegate OnAllocate;

    /**
     * Delegate that is invoked when this server has been deallocated.
     */
	UPROPERTY(BlueprintAssignable, Category="Multiplay | GameServer")
	FRSDeallocateDelegate OnDeallocate;

private:
	
	/**
	 * @brief Calls when connection messages have been received. 
	 * @param Reply The message body.
	 */
	void OnConnectReply(const RocketScienceMultiplaySDK::FConnectResult& Reply);

	/**
	 * @brief Calls when push messages have been received. Interprets the message as a server event. 
	 * @param Push The message body.
	 */
	void OnPublicationPush(const RocketScienceMultiplaySDK::FPublication& Push);
	 
private:
	/**
	 * @brief Callback invoked when we have received a response to the ReadyServer request.
	 * @param Response The response body.
	 */
	void OnReadyServer(const RocketScienceMultiplaySDK::ReadyServerResponse& Response);

	/**
	 * @brief Callback invoked when we have received a response to the UnreadyServer request.
	 * @param Response The response body.
	 */
	void OnUnreadyServer(const RocketScienceMultiplaySDK::UnreadyServerResponse& Response);

private:
	/**
	 * @brief Callback invoked when we have received a response to the PayloadAllocation request.
	 * @param Response The response body.
	 */
	void OnPayloadAllocation(const RocketScienceMultiplaySDK::PayloadAllocationResponse& Response);

	/**
	 * @brief Callback invoked when we have received a response to the PayloadToken request.
	 * @param Response The response body.
	 */
	void OnPayloadToken(const RocketScienceMultiplaySDK::PayloadTokenResponse& Response);

private:
    /**
     * Holds a reference to the delegate that will be invoked when ReadyServer is successful.
     */
	UPROPERTY()
	FRSReadyServerSuccessDelegate OnReadyServerSuccess;

    /**
     * Holds a reference to the delegate that will be invoked when ReadyServer is unsuccessful.
     */
	UPROPERTY()
	FRSReadyServerFailureDelegate OnReadyServerFailure;

    /**
     * Holds a reference to the delegate that will be invoked when UnreadyServer is successful.
     */
	UPROPERTY()
	FRSUnreadyServerSuccessDelegate OnUnreadyServerSuccess;

    /**
     * Holds a reference to the delegate that will be invoked when UnreadyServer is unsuccessful.
     */
	UPROPERTY()
	FRSUnreadyServerFailureDelegate OnUnreadyServerFailure;

    /**
     * Holds a reference to the delegate that will be invoked when PayloadAllocation is successful.
     */
	UPROPERTY()
	FRSPayloadAllocationSuccessDelegate OnPayloadAllocationSuccess;

    /**
     * Holds a reference to the delegate that will be invoked when PayloadAllocation is unsuccessful.
     */
	UPROPERTY()
	FRSPayloadAllocationFailureDelegate OnPayloadAllocationFailure;

    /**
     * Holds a reference to the delegate that will be invoked when PayloadToken is successful.
     */
	UPROPERTY()
	FRSPayloadTokenSuccessDelegate OnPayloadTokenSuccess;

    /**
     * Holds a reference to the delegate that will be invoked when PayloadToken is unsuccessful.
     */
	UPROPERTY()
	FRSPayloadTokenFailureDelegate OnPayloadTokenFailure;

    /**
     * A reference to the client's connection to Centrifuge.
     */
	TUniquePtr<RocketScienceMultiplaySDK::FCentrifugeClient> CentrifugeClient;

    /**
     * A reference to the OpenAPI Game Server API.
     */
	TUniquePtr<RocketScienceMultiplaySDK::OpenAPIGameServerApi> GameServerApi;

    /**
     * A reference to the OpenAPI Payload API.
     */
	TUniquePtr<RocketScienceMultiplaySDK::OpenAPIPayloadApi> PayloadApi;

    /**
     * The unique UUID of the allocation.
     */
	FGuid AllocationId;
};
