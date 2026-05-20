// Fill out your copyright notice in the Description page of Project Settings.


#include "RoomGameState.h"

#include "GameFramework/PlayerState.h"
#include "Net/UnrealNetwork.h"
#include "UE20252Network/Player/Room/RoomPlayerController.h"

ARoomGameState::ARoomGameState()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ARoomGameState::BeginPlay()
{
	Super::BeginPlay();

	GEngine->AddOnScreenDebugMessage(-1, 1000.f, FColor::Blue, TEXT("GameState Begin"));
}

void ARoomGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ARoomGameState, PlayerNames);
	DOREPLIFETIME(ARoomGameState, PlayerCount);
}

void ARoomGameState::OnRep_PlayerListChange()
{
	// auto* PC = Cast<ARoomPlayerController>(GetWorld()->GetFirstPlayerController());
	// if (IsValid(PC))
	// {
	// 	PC->RefreshPlayer();
	// }

	for (const auto& Player : PlayerArray)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1000.f, FColor::Blue,
		                                 FString::Printf(TEXT("%s Login"), *Player->GetName()));
	}
}

void ARoomGameState::NotifyPlayerListChange()
{
	// if (HasAuthority())
	// {
	// 	// OnRep_PlayerListChange();
	// }
}

void ARoomGameState::RefreshPlayer_Implementation()
{
	for (const auto& Player : PlayerArray)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1000.f, FColor::Blue,
		                                 FString::Printf(TEXT("%s Login"), *Player->GetName()));
	}
}

void ARoomGameState::TryRefreshPlayer()
{
	for (const auto& Player : PlayerArray)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1000.f, FColor::Red, Player->GetPlayerName());
	}
}
