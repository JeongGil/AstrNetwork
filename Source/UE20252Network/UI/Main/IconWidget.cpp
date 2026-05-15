// Fill out your copyright notice in the Description page of Project Settings.


#include "IconWidget.h"
#include "MainWidget.h"
#include "ItemInfoWidget.h"
#include "GameSlotWidget.h"
#include "InventoryWidget.h"
#include "../../Player/InventoryComponent.h"
#include "../../Etc/ItemObject.h"
#include "../../GameMode/UIGameInstanceSubsystem.h"
#include "IconDragDropOperation.h"

UIconWidget::UIconWidget(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer)
{
}

void UIconWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

FReply UIconWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry,
	const FPointerEvent& InMouseEvent)
{
	if (InMouseEvent.GetEffectingButton() == EKeys::RightMouseButton)
	{
		switch (mParentSlot->GetSlotType())
		{
		case ESlotWidgetType::Inventory:
			UseInventoryItem();
			break;
		case ESlotWidgetType::Quick:
			UseQuickSlotItem();
			break;
		}

		return FReply::Handled();
	}

	else if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		FEventReply EventReply = UWidgetBlueprintLibrary::DetectDragIfPressed(
			InMouseEvent, this, EKeys::LeftMouseButton);

		return EventReply.NativeReply;
	}

	return FReply::Unhandled();
}

FReply UIconWidget::NativeOnMouseButtonUp(const FGeometry& InGeometry,
	const FPointerEvent& InMouseEvent)
{
	return FReply::Unhandled();
}

FReply UIconWidget::NativeOnMouseMove(const FGeometry& InGeometry,
	const FPointerEvent& InMouseEvent)
{
	if (GetVisibility() == ESlateVisibility::Collapsed)
		return FReply::Unhandled();

	// UIGameInstanceSubsystem
	UGameInstance* GameInst = GetWorld()->GetGameInstance();

	// 테스트에 사용할 아이템 생성.
	UUIGameInstanceSubsystem* UISubSystem =
		GameInst->GetSubsystem<UUIGameInstanceSubsystem>();

	UMainWidget* MainWidget = UISubSystem->FindWidget<UMainWidget>(TEXT("MainWidget"));

	UItemInfoWidget* InfoWidget = MainWidget->GetItemInfoWidget();

	InfoWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	UCanvasPanelSlot* PanelSlot = Cast<UCanvasPanelSlot>(InfoWidget->Slot);

	// 부모 위젯을 꺼내온다.
	UWidget* ParentWidget = InfoWidget->GetParent();

	FVector2D	MousePos = InMouseEvent.GetScreenSpacePosition();

	FVector2D	WidgetPos = ParentWidget->GetCachedGeometry().AbsoluteToLocal(MousePos);

	WidgetPos.X += 10.0;

	PanelSlot->SetPosition(WidgetPos);

	return FReply::Handled();
}

void UIconWidget::NativeOnMouseEnter(const FGeometry& InGeometry,
	const FPointerEvent& InMouseEvent)
{
	UE_LOG(UELOG, Warning, TEXT("Mouse Enter"));

	switch (mParentSlot->GetSlotType())
	{
	case ESlotWidgetType::Inventory:
		InventoryItemData();
		break;
	case ESlotWidgetType::Quick:
		QuickSlotItemData();
		break;
	}
}

void UIconWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	UE_LOG(UELOG, Warning, TEXT("Mouse Leave"));

	// UIGameInstanceSubsystem
	UGameInstance* GameInst = GetWorld()->GetGameInstance();

	// 테스트에 사용할 아이템 생성.
	UUIGameInstanceSubsystem* UISubSystem =
		GameInst->GetSubsystem<UUIGameInstanceSubsystem>();

	UMainWidget* MainWidget = UISubSystem->FindWidget<UMainWidget>(TEXT("MainWidget"));

	UItemInfoWidget* InfoWidget = MainWidget->GetItemInfoWidget();

	InfoWidget->SetVisibility(ESlateVisibility::Collapsed);
}

void UIconWidget::NativeOnDragDetected(const FGeometry& InGeometry,
	const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	UIconDragDropOperation* DragOP = NewObject<UIconDragDropOperation>();

	DragOP->mDragIcon = this;
	DragOP->Pivot = EDragPivot::MouseDown;
	DragOP->mStartSlot = mParentSlot;

	// Drag할 때 보여줄 Icon을 새로 생성하여 보여주게 한다.
	UIconWidget* DragWidget = CreateWidget<UIconWidget>(GetWorld(), mDragClass);

	DragWidget->mIconImage->SetBrushFromTexture(mIconTexture);
	DragWidget->mIconImage->SetBrushColor(FLinearColor(1.f, 1.f, 1.f, 0.5f));
	DragWidget->EnableCountText(false);

	DragOP->DefaultDragVisual = DragWidget;

	OutOperation = DragOP;
}

void UIconWidget::SetIconImage(UTexture2D* Image)
{
	mIconTexture = Image;
	mIconImage->SetBrushFromTexture(Image);
}

void UIconWidget::SetCount(int32 Count)
{
	if (Count > 0)
		mCountText->SetText(FText::FromString(FString::FromInt(Count)));

	else
		mCountText->SetText(FText());
}

void UIconWidget::SetEquip(bool Equip)
{
	UE_LOG(UELOG, Warning, TEXT("UIconWidget SetItemEquip"));
	if (Equip)
	{
		mCountText->SetVisibility(ESlateVisibility::Visible);
		mCountText->SetText(FText::FromString(TEXT("장착")));
	}

	else
		mCountText->SetText(FText());
}

void UIconWidget::EnableCountText(bool Enable)
{
	if (Enable)
		mCountText->SetVisibility(ESlateVisibility::Visible);

	else
		mCountText->SetVisibility(ESlateVisibility::Collapsed);
}

void UIconWidget::InventoryItemData()
{
	// UIGameInstanceSubsystem
	UGameInstance* GameInst = GetWorld()->GetGameInstance();

	// 테스트에 사용할 아이템 생성.
	UUIGameInstanceSubsystem* UISubSystem =
		GameInst->GetSubsystem<UUIGameInstanceSubsystem>();

	UInventoryWidget* InventoryWidget = UISubSystem->FindWidget<UInventoryWidget>(TEXT("Inventory"));

	UInventoryComponent* Inventory = InventoryWidget->GetInventory();

	int32	SlotIndex = mParentSlot->GetSlotIndex();

	UItemObject* Item = Inventory->GetItem(SlotIndex);

	if (Item)
	{
		FString	ItemName = Item->GetItemName();
		EItemType	ItemType = Item->GetItemType();
		int32	ItemLevel = Item->GetItemLevel();
		FString	ItemDesc = Item->GetItemDesc();

		UMainWidget* MainWidget = UISubSystem->FindWidget<UMainWidget>(TEXT("MainWidget"));

		UItemInfoWidget* InfoWidget = MainWidget->GetItemInfoWidget();

		InfoWidget->SetItemName(ItemName);
		InfoWidget->SetItemType(ItemType);
		InfoWidget->SetItemLevel(ItemLevel);
		InfoWidget->SetItemDesc(ItemDesc);
		InfoWidget->SetItemOptions(Item->GetItemOptions());
	}
}

void UIconWidget::QuickSlotItemData()
{
}

void UIconWidget::UseInventoryItem()
{
	// UIGameInstanceSubsystem
	UGameInstance* GameInst = GetWorld()->GetGameInstance();

	// 테스트에 사용할 아이템 생성.
	UUIGameInstanceSubsystem* UISubSystem =
		GameInst->GetSubsystem<UUIGameInstanceSubsystem>();

	UInventoryWidget* InventoryWidget = UISubSystem->FindWidget<UInventoryWidget>(TEXT("Inventory"));

	UInventoryComponent* Inventory = InventoryWidget->GetInventory();

	Inventory->UseItem(mParentSlot->GetSlotIndex());
}

void UIconWidget::UseQuickSlotItem()
{
}
