// Fill out your copyright notice in the Description page of Project Settings.


#include "RoomGameState.h"

#include "UE20252Network/Player/Room/RoomPlayerState.h"

ARoomGameState::ARoomGameState()
{
	PrimaryActorTick.bCanEverTick = true;
}

TArray<ARoomPlayerState*> ARoomGameState::GetConnectedPlayers() const
{
	TArray<ARoomPlayerState*> ConnectedPlayers;

	for (auto& PS : PlayerArray)
	{
		auto* RoomPS = Cast<ARoomPlayerState>(PS);
		if (IsValid(RoomPS))
		{
			if (RoomPS->IsConnected())
			{
				ConnectedPlayers.Add(RoomPS);
			}
		}
	}

	ConnectedPlayers.Sort([](const auto& A, const auto& B)
	{
		return A.GetUserSlot() < B.GetUserSlot();
	});

	return ConnectedPlayers;
}

void ARoomGameState::BeginPlay()
{
	Super::BeginPlay();
}

void ARoomGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void ARoomGameState::NotifyPlayerListChange()
{
	if (OnPlayerListChanged.IsBound())
	{
		OnPlayerListChanged.Broadcast();
	}
}

void ARoomGameState::NotifyPlayerLogout_Implementation(const FString& PlayerName)
{
	GEngine->AddOnScreenDebugMessage(-1, 1000.f, FColor::Red, FString::Printf(TEXT("%s Logout"), *PlayerName));

	if (OnPlayerLogout.IsBound())
	{
		OnPlayerLogout.Broadcast(PlayerName);
	}

	// NotifyPlayerListChange();
}

void ARoomGameState::OnRep_ReplicatedHasBegunPlay()
{
	Super::OnRep_ReplicatedHasBegunPlay();

	NotifyPlayerListChange();
}
