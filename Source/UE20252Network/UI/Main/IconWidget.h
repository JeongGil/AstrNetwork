// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../GameInfo.h"
#include "../UIInfo.h"
#include "Blueprint/UserWidget.h"
#include "IconWidget.generated.h"

/**
 * 
 */
UCLASS()
class UE20252NETWORK_API UIconWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UIconWidget(const FObjectInitializer& ObjectInitializer);

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UBorder>	mIconImage;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock>	mCountText;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UIconWidget>	mDragClass;

	TObjectPtr<UTexture2D>		mIconTexture;

	class UGameSlotWidget* mParentSlot = nullptr;

public:
	void SetParentSlot(UGameSlotWidget* ParentSlot)
	{
		mParentSlot = ParentSlot;
	}

protected:
	virtual void NativeOnInitialized();
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);
	virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);
	virtual FReply NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent);
	virtual void NativeOnDragDetected(const FGeometry& InGeometry,
		const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation);

public:
	void SetIconImage(UTexture2D* Image);
	void SetCount(int32 Count);
	void SetEquip(bool Equip);
	void EnableCountText(bool Enable);

private:
	void InventoryItemData();
	void QuickSlotItemData();
	void UseInventoryItem();
	void UseQuickSlotItem();
};
