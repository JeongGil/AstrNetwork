// Fill out your copyright notice in the Description page of Project Settings.


#include "RoomGameMode.h"

#include "RoomGameState.h"
#include "UE20252Network/Player/Room/RoomPlayerController.h"

ARoomGameMode::ARoomGameMode()
{
	PrimaryActorTick.bCanEverTick = true;

	DefaultPawnClass = nullptr;
	PlayerStateClass = nullptr;
	PlayerControllerClass = ARoomPlayerController::StaticClass();
	GameStateClass = ARoomGameState::StaticClass();
}

APlayerController* ARoomGameMode::Login(UPlayer* NewPlayer, ENetRole InRemoteRole, const FString& Portal,
	const FString& Options, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage)
{
	APlayerController* Result = Super::Login(NewPlayer, InRemoteRole, Portal, Options, UniqueId, ErrorMessage);

	return Result;
}

void ARoomGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
}

void ARoomGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);
}
