// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../UI/BaseUserWidget.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "UIGameInstanceSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class UE20252NETWORK_API UUIGameInstanceSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
private:
	TMap<FString, TObjectPtr<UBaseUserWidget>>	mWidgetMap;

public:
	void AddWidget(const FString& Name, UBaseUserWidget* Widget);
	void RemoveWidget(const FString& Name);
	template <typename T>
	T* FindWidget(const FString& Name)
	{
		return Cast<T>(mWidgetMap.FindRef(Name));
	}
};
