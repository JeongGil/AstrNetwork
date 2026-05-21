// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerEntryWidget.generated.h"

class UTextBlock;
/**
 *
 */
UCLASS()
class UE20252NETWORK_API UPlayerEntryWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPlayerEntryWidget(const FObjectInitializer& ObjectInitializer);

protected:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> mPlayerName;

public:
	UFUNCTION(BlueprintCallable)
	void SetItem(UObject* Item);
};
