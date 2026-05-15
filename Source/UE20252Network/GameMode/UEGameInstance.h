// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "Engine/GameInstance.h"
#include "UEGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class UE20252NETWORK_API UUEGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UUEGameInstance();

private:
	FName	mNextLevelName;
	FString		mNextLevelPackagePath;
	FString		mOptionText;
	bool		mOption = false;

public:
	const FName& GetNextLevelName()	const
	{
		return mNextLevelName;
	}

	const FString& GetNextLevelPackagePath()	const
	{
		return mNextLevelPackagePath;
	}

	const FString& GetOptionText()	const
	{
		return mOptionText;
	}

	bool GetOption()	const
	{
		return mOption;
	}

public:
	void LevelLoading(const FName& LevelName, const FString& PackagePath,
		bool Option = false, const FString& OptionText = TEXT(""));
};
