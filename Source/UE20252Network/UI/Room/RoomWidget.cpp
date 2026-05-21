// Fill out your copyright notice in the Description page of Project Settings.


#include "RoomWidget.h"

#include "Components/Button.h"
#include "Components/ListView.h"
#include "Components/TextBlock.h"
#include "UE20252Network/Player/Room/PlayerEntryObject.h"
#include "UE20252Network/Player/Room/RoomPlayerController.h"
#include "UE20252Network/Player/Room/RoomPlayerState.h"

URoomWidget::URoomWidget(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer)
{
	mWidgetName = TEXT("RoomWidget");
}

void URoomWidget::RefreshPlayerList(TArray<ARoomPlayerState*> Array)
{
	GEngine->AddOnScreenDebugMessage(-1, 1000.f, FColor::Blue, TEXT("PlayerListChange"));

	mPlayerList->ClearListItems();

	for (auto& PS : Array)
	{
		auto* Item = NewObject<UPlayerEntryObject>();
		Item->PlayerName = PS->GetPlayerDisplayName();

		mPlayerList->AddItem(Item);
	}
}

void URoomWidget::PlayerLogout(TArray<ARoomPlayerState*> Array, const FString& LogoutPlayerName)
{
	mPlayerList->ClearListItems();

	for (auto& PS : Array)
	{
		if (PS->GetPlayerDisplayName() == LogoutPlayerName)
		{
			continue;
		}

		auto* Item = NewObject<UPlayerEntryObject>();
		Item->PlayerName = PS->GetPlayerDisplayName();

		mPlayerList->AddItem(Item);
	}
}

void URoomWidget::SetButtonText(const FString& Text)
{
	mButtonText->SetText(FText::FromString(Text));
}

void URoomWidget::SetButtonEnable(bool bEnable)
{
	mStartButton->SetIsEnabled(bEnable);
}

void URoomWidget::EnableReadyButton()
{
	ButtonType = ERoomButtonType::Ready;
}

void URoomWidget::ButtonClick()
{
	auto* PC = GetOwningPlayer<ARoomPlayerController>();
	if (ButtonType == ERoomButtonType::Ready)
	{
		if (IsValid(PC))
		{
			bReady = !bReady;
			PC->UserReady();
		}
	}
	else
	{
		if (IsValid(PC))
		{
			PC->TransitionMain();
		}
	}
}

void URoomWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	mStartButton->OnClicked.AddDynamic(this, &URoomWidget::ButtonClick);
}
