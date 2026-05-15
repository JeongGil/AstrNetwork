// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../BaseUserWidget.h"
#include "PlayerHUDWidget.generated.h"

/**
 * 
 */
UCLASS()
class UE20252NETWORK_API UPlayerHUDWidget : public UBaseUserWidget
{
	GENERATED_BODY()

public:
	UPlayerHUDWidget(const FObjectInitializer& ObjectInitializer);

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock>	mPlayerName;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar>	mHPBar;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar>	mMPBar;

protected:
	virtual void NativeOnInitialized();
	virtual void NativePreConstruct();
	virtual void NativeConstruct();

public:
	void SetPlayerName(const FString& Name);
	void SetHPPercent(float Percent);
	void SetMPPercent(float Percent);
};
