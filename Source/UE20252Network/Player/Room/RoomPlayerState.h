// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "RoomPlayerState.generated.h"

/**
 *
 */
UCLASS()
class UE20252NETWORK_API ARoomPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	ARoomPlayerState();

private:
	UPROPERTY(ReplicatedUsing=OnRep_PlayerInfo)
	FString PlayerDisplayName;

	UPROPERTY(ReplicatedUsing=OnRep_PlayerInfo)
	bool bConnected;

	UPROPERTY(Replicated)
	int32 UserSlotNumber;

	UPROPERTY(ReplicatedUsing=OnRep_Ready)
	bool bReady = false;

public:
	[[nodiscard]] bool IsReady() const
	{
		return bReady;
	}

	void TransitionReady();

	[[nodiscard]] FString GetPlayerDisplayName() const
	{
		return PlayerDisplayName;
	}

	void SetPlayerDisplayName(const FString& Name);

	[[nodiscard]] bool IsConnected() const
	{
		return bConnected;
	}

	void SetConnected(bool bConnect);

	[[nodiscard]] int32 GetUserSlot() const
	{
		return UserSlotNumber;
	}

	void SetUserSlot(int32 UserSlot)
	{
		this->UserSlotNumber = UserSlot;
	}

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void OnRep_PlayerInfo();

	void BroadcastPlayerListUpdate();

	UFUNCTION()
	void OnRep_Ready();
};
