// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerInfoWidget.h"

UPlayerInfoWidget::UPlayerInfoWidget(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer)
{
}

void UPlayerInfoWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void UPlayerInfoWidget::SetPlayerName(const FString& Name)
{
	mPlayerName->SetText(FText::FromString(Name));
}

void UPlayerInfoWidget::SetHPPercent(float Percent)
{
	mHPBar->SetPercent(Percent);
}
