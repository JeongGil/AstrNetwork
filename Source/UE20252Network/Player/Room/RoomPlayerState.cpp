// Fill out your copyright notice in the Description page of Project Settings.


#include "RoomPlayerState.h"

#include "Net/UnrealNetwork.h"
#include "UE20252Network/GameMode/RoomGameState.h"

ARoomPlayerState::ARoomPlayerState()
{
	bConnected = false;
	PlayerDisplayName = TEXT("Unknown");
}

void ARoomPlayerState::SetPlayerDisplayName(const FString& Name)
{
	if (HasAuthority())
	{
		PlayerDisplayName = Name;

		BroadcastPlayerListUpdate();
	}
}

void ARoomPlayerState::SetConnected(bool bConnect)
{
	if (HasAuthority())
	{
		bConnected = bConnect;

		BroadcastPlayerListUpdate();
	}
}

void ARoomPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ARoomPlayerState, PlayerDisplayName);
	DOREPLIFETIME(ARoomPlayerState, bConnected);
	DOREPLIFETIME(ARoomPlayerState, UserSlotNumber);
}

void ARoomPlayerState::BeginPlay()
{
	Super::BeginPlay();
}

void ARoomPlayerState::OnRep_PlayerInfo()
{
	BroadcastPlayerListUpdate();
}

void ARoomPlayerState::BroadcastPlayerListUpdate()
{
	auto* GS = GetWorld()->GetGameState<ARoomGameState>();
	if (IsValid(GS))
	{
		GS->NotifyPlayerListChange();
	}

}
