// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "LoginTimerHelper.generated.h"

class ARoomGameMode;
/**
 *
 */
UCLASS()
class UE20252NETWORK_API ULoginTimerHelper : public UObject
{
	GENERATED_BODY()

public:
	TWeakObjectPtr<APlayerController> TargetController;
	TWeakObjectPtr<ARoomGameMode> GameMode;
	FTimerHandle TimerHandle;

	void Execute();
};
