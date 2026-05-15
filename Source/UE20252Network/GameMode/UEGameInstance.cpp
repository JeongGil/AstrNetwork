// Fill out your copyright notice in the Description page of Project Settings.


#include "UEGameInstance.h"

UUEGameInstance::UUEGameInstance()
{
}

void UUEGameInstance::LevelLoading(const FName& LevelName, const FString& PackagePath,
	bool Option, const FString& OptionText)
{
	mNextLevelName = LevelName;
	mNextLevelPackagePath = PackagePath;
	mOption = Option;
	mOptionText = OptionText;

	UGameplayStatics::OpenLevel(GetWorld(), TEXT("Loading"));
}
