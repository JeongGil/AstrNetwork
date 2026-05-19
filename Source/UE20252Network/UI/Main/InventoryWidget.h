// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../BaseUserWidget.h"
#include "GameSlotWidget.h"
#include "InventoryWidget.generated.h"

class UInventoryComponent;

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
	TObjectPtr<UTextBlock> mGold;

	TArray<UGameSlotWidget*> mSlotArray;

	UInventoryComponent* mInventory;

	bool		mDragEnable = false;

	FVector2D	mDragStartPos;
	FVector2D	mWidgetPos;

public:
	FORCEINLINE UInventoryComponent* GetInventory()	const
	{
		return mInventory;
	}

protected:
	virtual void NativeOnInitialized() override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

public:
	void InitItemSlot(UInventoryComponent* Inventory);
	void InitInventory(UInventoryComponent* Inventory);

	void ChangeItem(UItemObject* Item, int32 Index);
	void ChangeGold(int32 Gold);
	void ChangeItemCount(int32 Index, int32 Count);
	void ChangeItemEquip(int32 Index, bool Equip);

public:
	UFUNCTION()
	void CloseButtonClick();
};
