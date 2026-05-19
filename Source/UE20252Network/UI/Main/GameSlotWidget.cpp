// Fill out your copyright notice in the Description page of Project Settings.


#include "GameSlotWidget.h"
#include "../../Etc/ItemObject.h"
#include "IconDragDropOperation.h"
#include "../../Player/InventoryComponent.h"
#include "../../GameMode/UIGameInstanceSubsystem.h"
#include "InventoryWidget.h"
#include "UE20252Network/GameMode/AssetGameInstanceSubsystem.h"

UGameSlotWidget::UGameSlotWidget(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer)
{
}

void UGameSlotWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	mIcon->SetParentSlot(this);
}

bool UGameSlotWidget::NativeOnDrop(const FGeometry& InGeometry,
                                   const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	UIconDragDropOperation* DragOP = Cast<UIconDragDropOperation>(InOperation);

	if (!DragOP)
		return false;

	if (DragOP->mStartSlot->mIndex == mIndex)
		return false;

	if (DragOP->mStartSlot->mSlotType == ESlotWidgetType::Inventory &&
		mSlotType == ESlotWidgetType::Inventory)
	{
		// UIGameInstanceSubsystem
		UGameInstance* GameInst = GetWorld()->GetGameInstance();

		// 테스트에 사용할 아이템 생성.
		UUIGameInstanceSubsystem* UISubSystem =
			GameInst->GetSubsystem<UUIGameInstanceSubsystem>();

		UInventoryWidget* InventoryWidget = UISubSystem->FindWidget<UInventoryWidget>(TEXT("Inventory"));

		UInventoryComponent* Inventory = InventoryWidget->GetInventory();

		Inventory->SwapItem(DragOP->mStartSlot->mIndex, mIndex);
	}

	return true;
}


void UGameSlotWidget::SetItem(class UItemObject* Item)
{
	if (IsValid(Item))
	{
		UGameInstance* GameInstance = GetWorld()->GetGameInstance();
		auto* AssetSubsystem = GameInstance->GetSubsystem<UAssetGameInstanceSubsystem>();

		if (IsValid(AssetSubsystem))
		{
			if (const FItemTableInfo* Info = AssetSubsystem->FindItemInfo(Item->GetItemRowName()))
			{
				mIcon->SetIconImage(Info->IconImage);
			}
			else
			{
				GEngine->AddOnScreenDebugMessage(-1, 1000.f, FColor::Red, TEXT("ItemInfo is nullptr"));
			}
		}

		if (Item->GetItemLayerType() == EItemWidgetLayerType::Layered)
		{
			mIcon->EnableCountText(true);
			SetItemCount(Item->GetItemCount());
		}
		else
		{
			mIcon->EnableCountText(false);
		}

		GEngine->AddOnScreenDebugMessage(-1, 1000.f, FColor::Red, TEXT("Icon Visible"));
		mIcon->SetVisibility(ESlateVisibility::Visible);
	}

	else
	{
		mIcon->SetIconImage(nullptr);
		mIcon->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UGameSlotWidget::SetItemCount(int32 Count)
{
	UE_LOG(UELOG, Warning, TEXT("ItemCount : %d"), Count);
	mIcon->SetCount(Count);
}

void UGameSlotWidget::SetItemEquip(bool Equip)
{
	UE_LOG(UELOG, Warning, TEXT("SetItemEquip"));
	mIcon->SetEquip(Equip);
}
