// Fill out your copyright notice in the Description page of Project Settings.


#include "MainWidget.h"

UMainWidget::UMainWidget(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer)
{
	mWidgetName = TEXT("MainWidget");
}

void UMainWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	mPlayerHUD = Cast<UPlayerHUDWidget>(GetWidgetFromName(TEXT("UB_PlayerHUD")));

	if (IsValid(mPlayerHUD))
	{
		UE_LOG(UELOG, Warning, TEXT("PlayerHUD Widget"));
	}
}
