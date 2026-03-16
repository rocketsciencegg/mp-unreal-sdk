/*
 * #####################################################################################
 *  Rocket Science Multiplay Plugin for Unreal Engine Copyright © 2026 Rocket Science Group
 * #####################################################################################
 */

#pragma once

#include "CoreMinimal.h"
#include "Common/UdpSocketReceiver.h"
#include "Common/UdpSocketBuilder.h"
#include "RSMultiplaySubsystemBase.h"
#include "RSMultiplayServerQueryHandlerSubsystem.generated.h"

/** 
  * @brief Subsystem responsible for handling Multiplay server queries. 
  */
UCLASS()
class ROCKETSCIENCEMULTIPLAYSDK_API URSMultiplayServerQueryHandlerSubsystem : public URSMultiplaySubsystemBase
{
	GENERATED_BODY()

public:
	/**
	 * Subsystem functions, overrides from USubsystem.
	 */
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

public:
	/**
	 * @brief Start listening for Multiplay server queries.
	 * @return A bool indicating whether the query port was bound.
	 */
	UFUNCTION(BlueprintCallable, Category="Multiplay | ServerQuery")
	bool Connect();

	/**
	 * @brief Stop listening for Multiplay server queries. 
	 */
	UFUNCTION(BlueprintCallable, Category="Multiplay | ServerQuery")
	void Disconnect();

	/**
	 * @brief Indicates whether the server is actively listening for server query requests.
	 * @return A bool indicating whether the server is actively listening for server query requests.
	 */
	UFUNCTION(BlueprintCallable, Category="Multiplay | ServerQuery")
	bool IsConnected() const;
	
	/**
	 * @brief	Gets the current number of players connected to the server.
	 * 
	 * The current players value is represented using a uint16 in the server query protocol.
     * The maximum representable value for the current number of players is 65535.
	 * 
	 * @return The current number of players connected to the server.
	 */
	UFUNCTION(BlueprintGetter, Category="Multiplay | ServerQuery")
	const int32& GetCurrentPlayers() const;

	/**
	 * @brief Atomically increment the current number of players connected to the server.
	 * 
	 * This method should be invoked whenever a player joins the match.
	 * 
     * The current players value is represented using a uint16 in the server query protocol.
     * The maximum representable value for the current number of players is 65535.
	 * 
	 * See SetCurrentPlayers() as an alternative for overwriting the current value.
	 */
	UFUNCTION(BlueprintCallable, Category="Multiplay | ServerQuery")
	void IncrementCurrentPlayers();

	/**
	 * @brief Atomically decrement the current number of players connected to the server.
	 * 
	 * This method should be invoked whenever a player leaves the match.
	 * 
     * The current players value is represented using a uint16 in the server query protocol.
     * The maximum representable value for the current number of players is 65535.
	 * 
	 * See SetCurrentPlayers() as an alternative for overwriting the current value.
	 */
	UFUNCTION(BlueprintCallable, Category="Multiplay | ServerQuery")
	void DecrementCurrentPlayers();

	/**
	 * @brief Sets the current number of players connected to the server.
	 * 
	 * This method should be invoked whenever a player joins the match or leaves the match.
	 * 
	 * The current players value is represented using a uint16 in the server query protocol.
	 * The maximum representable value for the current number of players is 65535.
	 * 
	 * See IncrementCurrentPlayers() and DecrementCurrentPlayers() for atomic alternatives to SetCurrentPlayers(int32).
	 * 
	 * @param Value The current number of players.
	 */
	UFUNCTION(BlueprintSetter, Category="Multiplay | ServerQuery")
	void SetCurrentPlayers(int32 Value);

	/**
	 * @brief Gets the maximum number of players allowed on the server.
	 * @return The maximum number of players allowed.
	 */
	UFUNCTION(BlueprintGetter, Category="Multiplay | ServerQuery")
	const int32& GetMaxPlayers() const;

	/**
	 * @brief Sets the maximum amount of players allowed on the server.
	 *        Note: Only values up to 65535 (max uint16) are supported.
	 * @param Value The maximum number of players.
	 */
	UFUNCTION(BlueprintSetter, Category="Multiplay | ServerQuery")
	void SetMaxPlayers(int32 Value);

	/**
	 * @brief Gets the server's name. 
	 * @return The server's name.
	 */
	UFUNCTION(BlueprintGetter, Category="Multiplay | ServerQuery")
	const FString& GetServerName() const;

	/**
	 * @brief Sets the server's name. 
	 *        Note: Must be under 255 characters.
	 * @param Value The server's name.
	 */
	UFUNCTION(BlueprintSetter, Category="Multiplay | ServerQuery")
	void SetServerName(FString Value);

	/**
	 * @brief Gets the game type that the server is currently being allocated for.
	 * @return The server's game type name.
	 */
	UFUNCTION(BlueprintGetter, Category="Multiplay | ServerQuery")
	const FString& GetGameType() const;

	/**
	 * @brief Sets the server's game type.
	 *        Note: Must be under 255 characters.
	 * @param Value The server's game type.
	 */
	UFUNCTION(BlueprintSetter, Category="Multiplay | ServerQuery")
	void SetGameType(FString Value);

	/**
	 * @brief Gets the build ID of the server.
	 * @return The server's build ID.
	 */
	UFUNCTION(BlueprintGetter, Category="Multiplay | ServerQuery")
	const FString& GetBuildId() const;

	/**
	 * @brief Sets the server's build ID.
	 *        Note: Must be under 255 characters. Wide characters are not supported.
	 * @param Value The server's build ID.
	 */
	UFUNCTION(BlueprintSetter, Category="Multiplay | ServerQuery")
	void SetBuildId(FString Value);

	/**
	 * @brief Gets the map the server is currently on.
	 * @return The name of the server's current map.
	 */
	UFUNCTION(BlueprintGetter, Category="Multiplay | ServerQuery")
	const FString& GetMap() const;

	/**
	 * @brief Sets the server's map.
	 *        Note: Must be under 255 characters.
	 * @param Value The server's map.
	 */
	UFUNCTION(BlueprintSetter, Category="Multiplay | ServerQuery")
	void SetMap(FString Value);

	/**
	 * @brief Gets the SQP port that the server is listening on.
	 * @return The server's port.
	 */
	UFUNCTION(BlueprintGetter, Category="Multiplay | ServerQuery")
	const int32& GetPort() const;

	/**
	 * @brief Sets the server's SQP port that it listens on.
	 *        Note: Only values up to 65535 (max uint16) are supported.
	 * @param Value The server's port.
	 */
	UFUNCTION(BlueprintSetter, Category="Multiplay | ServerQuery")
	void SetPort(int32 Value);

protected:
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;

private:

	/**
	 * @brief Accepts requests over SQP and answers to them with response packets. 
	 *        Packets that are too small will be rejected.
	 * @param ArrayReaderPtr The packet's data. 
	 * @param EndPt The client's IPv4 endpoint.
	 */
	void ReceiveSQPData(const FArrayReaderPtr& ArrayReaderPtr, const FIPv4Endpoint& EndPt);

	/**
	  * @brief Sends a challenge packet to a client that has sent a challenge request. 
	  *        Only client addresses that have no pending requests will be valid. 
	  * @param FromAddress The client's IPv4 address.
	  */
	void SendSQPChallengePacket(TSharedRef<FInternetAddr> FromAddress);

	/**
	 * @brief Sends a query packet to a client that has sent a query request. 
	 *        Only client addresses that have issued a challenge token will be valid.
	 * @param FromAddress The client's IPv4 address.
	 */
	void SendSQPQueryPacket(const FArrayReaderPtr& ArrayReaderPtr, TSharedRef<FInternetAddr> FromAddress);

private:
	static constexpr int32 kMaxStringLength = 255;

    /**
     * A reference to the server's SQP web socket.
     */
	FSocket* QuerySocket;

    /**
     * A reference to the UDP receiver that retrieves UDP/IP packets.
     */
	TUniquePtr<FUdpSocketReceiver> UDPReceiver;

    /**
     * A mapping of IPv4 addresses to challenge token IDs.
     */
	TMap<FString, uint32> FSQPChallengeTokens;

    /**
     * Contains the number of players on the server.
     */
	UPROPERTY(BlueprintGetter = GetCurrentPlayers, BlueprintSetter = SetCurrentPlayers, Category="Multiplay | ServerQuery", meta=(AllowPrivateAccess = "true"))
	int32 CurrentPlayers;

    /**
     * Contains the maximum number of players the server supports.
     */
	UPROPERTY(BlueprintGetter = GetMaxPlayers, BlueprintSetter = SetMaxPlayers, Category="Multiplay | ServerQuery", meta=(AllowPrivateAccess = "true"))
	int32 MaxPlayers;

    /**
     * Contains the name of the server.
     */
	UPROPERTY(BlueprintGetter = GetServerName, BlueprintSetter = SetServerName, Category="Multiplay | ServerQuery", meta=(AllowPrivateAccess = "true"))
	FString ServerName;

    /**
     * Contains the type of game on the server.
     */
	UPROPERTY(BlueprintGetter = GetGameType, BlueprintSetter = SetGameType, Category="Multiplay | ServerQuery", meta=(AllowPrivateAccess = "true"))
	FString GameType;

    /**
     * Contains the version number or build ID of the server.
     */
	UPROPERTY(BlueprintGetter = GetBuildId, BlueprintSetter = SetBuildId, Category="Multiplay | ServerQuery", meta=(AllowPrivateAccess = "true"))
	FString BuildId;

    /**
     * Contains the map the server currently has loaded.
     */
	UPROPERTY(BlueprintGetter = GetMap, BlueprintSetter = SetMap, Category="Multiplay | ServerQuery", meta=(AllowPrivateAccess = "true"))
	FString Map;

    /**
     * Contains the game port the server has exposed.
     */
	UPROPERTY(BlueprintGetter = GetPort, BlueprintSetter = SetPort, Category="Multiplay | ServerQuery", meta=(AllowPrivateAccess = "true"))
	int32 Port;
};
