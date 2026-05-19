// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "RoomGameState.generated.h"

/**
 *
 */
UCLASS()
class UE20252NETWORK_API ARoomGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	ARoomGameState();

protected:
	UPROPERTY(Replicated)
	TArray<FString> PlayerNames;

	virtual void BeginPlay() override;

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
