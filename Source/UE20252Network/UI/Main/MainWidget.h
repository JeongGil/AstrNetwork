// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../BaseUserWidget.h"
#include "ItemInfoWidget.h"
#include "PlayerHUDWidget.h"
#include "MainWidget.generated.h"

/**
 * 
 */
UCLASS()
class UE20252NETWORK_API UMainWidget : public UBaseUserWidget
{
	GENERATED_BODY()

public:
	UMainWidget(const FObjectInitializer& ObjectInitializer);

protected:
	UPROPERTY()
	TObjectPtr<UPlayerHUDWidget>	mPlayerHUD;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UItemInfoWidget>	mItemInfo;

public:
	UPlayerHUDWidget* GetPlayerHUDWidget()	const
	{
		return mPlayerHUD;
	}

	UItemInfoWidget* GetItemInfoWidget()	const
	{
		return mItemInfo;
	}

protected:
	virtual void NativeOnInitialized();
};
