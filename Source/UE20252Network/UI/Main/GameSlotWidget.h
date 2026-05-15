// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "IconWidget.h"
#include "Blueprint/UserWidget.h"
#include "GameSlotWidget.generated.h"

/**
 * 
 */
UCLASS()
class UE20252NETWORK_API UGameSlotWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UGameSlotWidget(const FObjectInitializer& ObjectInitializer);

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UIconWidget>	mIcon;

	ESlotWidgetType	mSlotType = ESlotWidgetType::Inventory;

	int32			mIndex = 0;

protected:
	virtual void NativeOnInitialized();
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation);

public:
	ESlotWidgetType GetSlotType()	const
	{
		return mSlotType;
	}

	int32 GetSlotIndex()	const
	{
		return mIndex;
	}

public:
	void SetSlotType(ESlotWidgetType SlotType)
	{
		mSlotType = SlotType;
	}

	void SetIndex(int32 Index)
	{
		mIndex = Index;
	}

	void SetItem(class UItemObject* Item);
	void SetItemCount(int32 Count);
	void SetItemEquip(bool Equip);
};
