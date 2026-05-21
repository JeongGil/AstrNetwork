// Fill out your copyright notice in the Description page of Project Settings.


#include "RoomGameMode.h"

#include "LoginTimerHelper.h"
#include "RoomGameState.h"
#include "UE20252Network/Player/Room/RoomPlayerController.h"
#include "UE20252Network/Player/Room/RoomPlayerState.h"

ARoomGameMode::ARoomGameMode()
{
	PrimaryActorTick.bCanEverTick = true;

	DefaultPawnClass = nullptr;
	PlayerStateClass = ARoomPlayerState::StaticClass();
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

	auto* Helper = NewObject<ULoginTimerHelper>(this);
	Helper->GameMode = this;
	Helper->TargetController = NewPlayer;

	GetWorldTimerManager().SetTimer(Helper->TimerHandle, Helper, &ULoginTimerHelper::Execute, 0.1f, false);
}

void ARoomGameMode::Logout(AController* Exiting)
{
	auto* GS = GetGameState<ARoomGameState>();
	if (IsValid(GS))
	{
		const auto* PC = Cast<APlayerController>(Exiting);
		if (IsValid(PC))
		{
			const auto* PS = PC->GetPlayerState<ARoomPlayerState>();
			if (IsValid(PS))
			{
				GS->NotifyPlayerLogout(PS->GetPlayerDisplayName());
			}
		}
	}

	Super::Logout(Exiting);
}

void ARoomGameMode::LoginPlayerState(APlayerController* NewPlayer)
{
	auto* PS = NewPlayer->GetPlayerState<ARoomPlayerState>();
	if (IsValid(PS))
	{
		PS->SetUserSlot(++PlayerJoinOrder);

		const FString PlayerName = FString::Printf(TEXT("Player_%d"), PlayerJoinOrder);
		PS->SetPlayerDisplayName(PlayerName);
		PS->SetConnected(true);
	}
}
