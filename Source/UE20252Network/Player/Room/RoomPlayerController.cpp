// Fill out your copyright notice in the Description page of Project Settings.


#include "RoomPlayerController.h"

#include "Blueprint/UserWidget.h"
#include "GameFramework/PlayerState.h"
#include "UE20252Network/GameMode/RoomGameState.h"
#include "UE20252Network/UI/Room/RoomWidget.h"

ARoomPlayerController::ARoomPlayerController()
{
	bShowMouseCursor = true;
}

void ARoomPlayerController::RegisterGameState()
{
	auto* RoomState = GetWorld()->GetGameState<ARoomGameState>();
	if (IsValid(RoomState))
	{
		RoomState->OnPlayerListChanged.AddDynamic(this, &ARoomPlayerController::OnPlayerListChanged);

		RoomState->OnPlayerLogout.AddDynamic(this, &ARoomPlayerController::OnPlayerLogout);

		OnPlayerListChanged();

		GetWorldTimerManager().ClearTimer(TimerHandle);
	}
}

void ARoomPlayerController::OnPlayerListChanged()
{
	if (IsValid(RoomWidget))
	{
		auto* RoomState = GetWorld()->GetGameState<ARoomGameState>();
		RoomWidget->RefreshPlayerList(RoomState->GetConnectedPlayers());
	}
}

void ARoomPlayerController::OnPlayerLogout(const FString& PlayerName)
{
	if (IsValid(RoomWidget))
	{
		auto* RoomState = GetWorld()->GetGameState<ARoomGameState>();
		RoomWidget->PlayerLogout(RoomState->GetConnectedPlayers(), PlayerName);
	}
}

void ARoomPlayerController::BeginPlay()
{
	Super::BeginPlay();

	FInputModeGameAndUI InputMode;
	SetInputMode(InputMode);

	if (IsLocalPlayerController())
	{
		GEngine->AddOnScreenDebugMessage(-1, 1000.f, FColor::Red, TEXT("WidgetCreate"));

		auto* RoomWidgetClass = LoadClass<URoomWidget>(
			this, TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UI/Room/WB_Room.WB_Room_C'"));
		if (IsValid(RoomWidgetClass))
		{
			RoomWidget = CreateWidget<URoomWidget>(this, RoomWidgetClass);
			if (IsValid(RoomWidget))
			{
				RoomWidget->AddToViewport();
			}
		}

		GetWorldTimerManager().SetTimer(TimerHandle, this, &ARoomPlayerController::RegisterGameState, 0.1f, true);
	}
}

void ARoomPlayerController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);
}

void ARoomPlayerController::OnUnPossess()
{
	Super::OnUnPossess();
}
