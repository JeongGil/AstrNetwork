// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../GameInfo.h"
#include "../UIInfo.h"
#include "Blueprint/UserWidget.h"
#include "ItemOptionWidget.generated.h"

/**
 * 
 */
UCLASS()
class UE20252NETWORK_API UItemOptionWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UItemOptionWidget(const FObjectInitializer& ObjectInitializer);

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock>	mOptionName;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock>	mOption;

public:
	// 블루프린트에서 호출가능한 함수를 만든다.
	UFUNCTION(BlueprintCallable)
	void SetItemData(UObject* ItemData);
};
