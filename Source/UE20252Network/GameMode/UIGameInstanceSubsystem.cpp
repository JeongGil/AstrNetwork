// Fill out your copyright notice in the Description page of Project Settings.


#include "UIGameInstanceSubsystem.h"

void UUIGameInstanceSubsystem::AddWidget(const FString& Name, UBaseUserWidget* Widget)
{
	if (mWidgetMap.Contains(Name))
		return;

	mWidgetMap.Add(Name, Widget);
}

void UUIGameInstanceSubsystem::RemoveWidget(const FString& Name)
{
	if (!mWidgetMap.Contains(Name))
		return;

	mWidgetMap.Remove(Name);
}
