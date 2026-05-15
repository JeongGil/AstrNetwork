// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../BaseUserWidget.h"
#include "GameSlotWidget.h"
#include "InventoryWidget.generated.h"

UCLASS()
class UE20252NETWORK_API UInventoryWidget : public UBaseUserWidget
{
	GENERATED_BODY()

public:
	UInventoryWidget(const FObjectInitializer& ObjectInitializer);

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UBorder>	mTitleBar;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton>	mCloseButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock>	mGold;

	TArray<UGameSlotWidget*>	mSlotArray;

	class UInventoryComponent* mInventory;

	bool		mDragEnable = false;

	FVector2D	mDragStartPos;
	FVector2D	mWidgetPos;

public:
	class UInventoryComponent* GetInventory()	const
	{
		return mInventory;
	}

protected:
	virtual void NativeOnInitialized();
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);
	virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);
	virtual FReply NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);

public:
	void InitInventory(class UInventoryComponent* Inventory);

	void ChangeItem(class UItemObject* Item, int32 Index);
	void ChangeGold(int32 Gold);
	void ChangeItemCount(int32 Index, int32 Count);
	void ChangeItemEquip(int32 Index, bool Equip);

public:
	UFUNCTION()
	void CloseButtonClick();
};
