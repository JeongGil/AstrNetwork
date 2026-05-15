// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../GameInfo.h"
#include "../UiInfo.h"
#include "ItemOptionData.h"
#include "Blueprint/UserWidget.h"
#include "ItemInfoWidget.generated.h"

/**
 * 
 */
UCLASS()
class UE20252NETWORK_API UItemInfoWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UItemInfoWidget(const FObjectInitializer& ObjectInitializer);

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock>	mItemName;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock>	mItemType;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock>	mItemLevel;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock>	mItemDesc;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UListView>	mOptionList;

public:
	void SetItemName(const FString& ItemName)
	{
		mItemName->SetText(FText::FromString(ItemName));
	}

	void SetItemType(EItemType ItemType)
	{
		FString	ItemName = GetItemTypeName(ItemType);

		mItemType->SetText(FText::FromString(ItemName));
	}

	void SetItemLevel(int32 Level)
	{
		FString	ItemName = TEXT("제한레벨 : ") + FString::FromInt(Level);

		mItemLevel->SetText(FText::FromString(ItemName));
	}

	void SetItemDesc(const FString& Desc)
	{
		mItemDesc->SetText(FText::FromString(Desc));
	}

	void SetItemOptions(const TArray<FItemOption>& Options)
	{
		mOptionList->ClearListItems();

		for (const FItemOption& Option : Options)
		{
			FString	OptionName = GetItemOptionTypeName(Option.Type);

			UItemOptionData* OptionData = NewObject<UItemOptionData>();

			OptionData->SetOptionName(OptionName);
			OptionData->SetOption(Option.Option);

			mOptionList->AddItem(OptionData);
		}
	}
};
