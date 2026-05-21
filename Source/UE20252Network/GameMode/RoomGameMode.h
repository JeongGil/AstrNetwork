// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "RoomGameMode.generated.h"

/**
 *
 */
UCLASS()
class UE20252NETWORK_API ARoomGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ARoomGameMode();

private:
	int32 PlayerJoinOrder = 0;

public:
	virtual APlayerController* Login(UPlayer* NewPlayer, ENetRole InRemoteRole, const FString& Portal,
		const FString& Options, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage) override;
	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void Logout(AController* Exiting) override;

	void LoginPlayerState(APlayerController* NewPlayer);
};
