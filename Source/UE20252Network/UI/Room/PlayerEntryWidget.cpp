// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerEntryWidget.h"

#include "Components/TextBlock.h"
#include "UE20252Network/Player/Room/PlayerEntryObject.h"

UPlayerEntryWidget::UPlayerEntryWidget(const FObjectInitializer& ObjectInitializer) :
		Super(ObjectInitializer)
{
}

void UPlayerEntryWidget::SetItem(UObject* Item)
{
	auto* Entry = Cast<UPlayerEntryObject>(Item);
	mPlayerName->SetText(FText::FromString(Entry->PlayerName));
}
