// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../GameInfo.h"
#include "../UIInfo.h"
#include "Blueprint/UserWidget.h"
#include "PlayerInfoWidget.generated.h"

UCLASS()
class UE20252NETWORK_API UPlayerInfoWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPlayerInfoWidget(const FObjectInitializer& ObjectInitializer);

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock>	mPlayerName;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar>	mHPBar;

protected:
	virtual void NativeOnInitialized();

public:
	void SetPlayerName(const FString& Name);
	void SetHPPercent(float Percent);
};
