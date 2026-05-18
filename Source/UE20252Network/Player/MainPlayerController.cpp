// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayerController.h"
#include "../UI/Main/MainWidget.h"

AMainPlayerController::AMainPlayerController()
{
	PrimaryActorTick.bCanEverTick = true;

	// 마우스 커서를 보이게 한다.
	bShowMouseCursor = true;
}

void AMainPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// 입력 모드를 세팅한다.
	// FInputModeGameOnly
	// FInputModeGameAndUI
	// FInputModeUIOnly
	FInputModeGameAndUI InputMode;
	SetInputMode(InputMode);

	if (IsLocalPlayerController())
	{
		const TSubclassOf<UMainWidget> MainWidgetClass = LoadClass<UMainWidget>(
			this, TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UI/Main/WB_Main.WB_Main_C'"));

		mMainWidget = CreateWidget<UMainWidget>(this, MainWidgetClass);

		if (IsValid(mMainWidget))
		{
			mMainWidget->AddToViewport();
		}
	}

	// CurrentMouseCursor = EMouseCursor::TextEditBeam;
}

void AMainPlayerController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);
}

void AMainPlayerController::OnUnPossess()
{
	Super::OnUnPossess();
}

void AMainPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FHitResult Hit;
	bool Pick = GetHitResultUnderCursor(ECollisionChannel::ECC_GameTraceChannel5, true,
	                                    Hit);

	// if (Pick)
	// {
	// 	/*GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, Hit.ImpactPoint.ToString());*/
	// }
}
