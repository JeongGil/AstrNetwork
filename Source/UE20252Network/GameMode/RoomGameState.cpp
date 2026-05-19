// Fill out your copyright notice in the Description page of Project Settings.


#include "RoomGameState.h"

#include "Net/UnrealNetwork.h"

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
}
