// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerHUDWidget.h"

UPlayerHUDWidget::UPlayerHUDWidget(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer)
{
	mWidgetName = TEXT("PlayerHUD");
}

void UPlayerHUDWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void UPlayerHUDWidget::NativePreConstruct()
{
	Super::NativePreConstruct();
}

void UPlayerHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UPlayerHUDWidget::SetPlayerName(const FString& Name)
{
	mPlayerName->SetText(FText::FromString(Name));
}

void UPlayerHUDWidget::SetHPPercent(float Percent)
{
	mHPBar->SetPercent(Percent);
}

void UPlayerHUDWidget::SetMPPercent(float Percent)
{
	mMPBar->SetPercent(Percent);
}
