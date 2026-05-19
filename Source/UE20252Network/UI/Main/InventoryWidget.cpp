// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryWidget.h"
#include "GameSlotWidget.h"
#include "../../Etc/ItemObject.h"
#include "../../Player/InventoryComponent.h"

UInventoryWidget::UInventoryWidget(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer)
{
	mWidgetName = TEXT("Inventory");
}

void UInventoryWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	mCloseButton->OnClicked.AddDynamic(this, &UInventoryWidget::CloseButtonClick);
}

FReply UInventoryWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (InMouseEvent.GetEffectingButton() != EKeys::LeftMouseButton)
		return FReply::Unhandled();

	// 마우스 위치가 TitleBar 안이 아니라면 처리하지 않는다.
	if (!mTitleBar->GetCachedGeometry().IsUnderLocation(
		InMouseEvent.GetScreenSpacePosition()))
		return FReply::Unhandled();

	mDragEnable = true;

	// Drag 시작시 위치
	mDragStartPos = InMouseEvent.GetScreenSpacePosition();

	mWidgetPos = GetRenderTransform().Translation;

	// Handled() 로 처리를 하여 이 위젯이 마우스 클릭 이벤트를 처리했다는 의미가 된다.
	// CaptureMouse 함수로 TakeWidget을 지정하는데 마우스를 누른 뒤부터는 커서가 위젯
	// 밖으로 나가도 이 위젯이 계속 마우스 이벤트를 받게 한다.
	return FReply::Handled().CaptureMouse(TakeWidget());
}

FReply UInventoryWidget::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (InMouseEvent.GetEffectingButton() != EKeys::LeftMouseButton)
		return FReply::Unhandled();

	mDragEnable = false;

	return FReply::Handled().ReleaseMouseCapture();
}

FReply UInventoryWidget::NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (!mDragEnable)
		return FReply::Unhandled();

	FVector2D CurrentMousePos = InMouseEvent.GetScreenSpacePosition();

	FVector2D MouseDelta = CurrentMousePos - mDragStartPos;

	// DPI Scale을 얻어온다.
	float ViewScale = UWidgetLayoutLibrary::GetViewportScale(this);

	FVector2D WidgetPos = mWidgetPos + (MouseDelta / ViewScale);

	SetRenderTranslation(WidgetPos);

	return FReply::Handled();
}

void UInventoryWidget::InitInventory(UInventoryComponent* Inventory)
{
	if (!IsValid(Inventory))
	{
		return;
	}

	if (mInventory == Inventory && bIsInventoryInitialized)
	{
		return;
	}

	bIsInventoryInitialized = true;
	mInventory = Inventory;

	mSlotArray.Empty();

	int32 MaxCount = mInventory->GetInventoryMax();

	for (int32 i = 0; i < MaxCount; ++i)
	{
		FString SlotName = TEXT("WB_GameSlot_") + FString::FromInt(i + 1);

		UGameSlotWidget* InvenSlot = Cast<UGameSlotWidget>(GetWidgetFromName(*SlotName));

		//InvenSlot->SetInventoryComponent(mInventory);
		InvenSlot->SetIndex(i);
		InvenSlot->SetItem(nullptr);

		mSlotArray.Add(InvenSlot);
	}

	if (!mInventory->mItemChange.IsBound())
	{
		mInventory->mItemChange.AddUObject(this, &UInventoryWidget::ChangeItem);
	}

	if (!mInventory->mGoldChange.IsBound())
	{
		mInventory->mGoldChange.AddUObject(this, &UInventoryWidget::ChangeGold);
	}

	if (!mInventory->mItemCountChange.IsBound())
	{
		mInventory->mItemCountChange.AddUObject(this, &UInventoryWidget::ChangeItemCount);
	}

	if (!mInventory->mItemEquipChange.IsBound())
	{
		mInventory->mItemEquipChange.AddUObject(this, &UInventoryWidget::ChangeItemEquip);
	}
}

void UInventoryWidget::ChangeItem(UItemObject* Item, int32 Index)
{
	mSlotArray[Index]->SetItem(Item);
}

void UInventoryWidget::ChangeGold(int32 Gold)
{
	FString Text = TEXT("Gold : ") + FString::FromInt(Gold);

	mGold->SetText(FText::FromString(Text));
}

void UInventoryWidget::ChangeItemCount(int32 Index, int32 Count)
{
	UE_LOG(UELOG, Warning, TEXT("Item : %d Count : %d"), Index, Count);
	mSlotArray[Index]->SetItemCount(Count);
}

void UInventoryWidget::ChangeItemEquip(int32 Index, bool Equip)
{
	UE_LOG(UELOG, Warning, TEXT("ChangeItemEquip"));
	mSlotArray[Index]->SetItemEquip(Equip);
}

void UInventoryWidget::CloseButtonClick()
{
	SetVisibility(ESlateVisibility::Collapsed);
}
