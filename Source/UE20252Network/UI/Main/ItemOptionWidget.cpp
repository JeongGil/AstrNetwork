// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemOptionWidget.h"
#include "ItemOptionData.h"

UItemOptionWidget::UItemOptionWidget(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer)
{
}

void UItemOptionWidget::SetItemData(UObject* ItemData)
{
	UItemOptionData* Data = Cast<UItemOptionData>(ItemData);

	mOptionName->SetText(FText::FromString(Data->GetOptionName()));
	mOption->SetText(FText::FromString(FString::Printf(TEXT("%.2f"), Data->GetOption())));
}
