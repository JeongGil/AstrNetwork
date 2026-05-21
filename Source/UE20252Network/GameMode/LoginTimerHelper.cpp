// Fill out your copyright notice in the Description page of Project Settings.


#include "LoginTimerHelper.h"

#include "RoomGameMode.h"
#include "GameFramework/PlayerState.h"

void ULoginTimerHelper::Execute()
{
	if (!GameMode.IsValid())
	{
		return;
	}

	if (!TargetController.IsValid() || !IsValid(TargetController.Get()->PlayerState))
	{
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle);

		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ULoginTimerHelper::Execute, 0.1f, false);

		return;
	}

	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);

	GameMode.Get()->LoginPlayerState(TargetController.Get());

	MarkAsGarbage();
}
