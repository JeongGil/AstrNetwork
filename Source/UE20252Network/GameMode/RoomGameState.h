// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "RoomGameState.generated.h"

class ARoomPlayerState;
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlayerListChange);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerLogout, const FString&, PlayerName);

/**
 *
 */
UCLASS()
class UE20252NETWORK_API ARoomGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	ARoomGameState();

	FOnPlayerListChange OnPlayerListChanged;
	FOnPlayerLogout OnPlayerLogout;

	TArray<ARoomPlayerState*> GetConnectedPlayers() const;

protected:
	virtual void BeginPlay() override;

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	void NotifyPlayerListChange();

	UFUNCTION(NetMulticast, Reliable)
	void NotifyPlayerLogout(const FString& PlayerName);
	void NotifyPlayerLogout_Implementation(const FString& PlayerName);

protected:
	virtual void OnRep_ReplicatedHasBegunPlay() override;
};
