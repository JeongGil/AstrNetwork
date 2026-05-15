// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../GameInfo.h"
#include "UObject/Object.h"
#include "ItemOptionData.generated.h"

/**
 * 
 */
UCLASS()
class UE20252NETWORK_API UItemOptionData : public UObject
{
	GENERATED_BODY()

public:
	UItemOptionData();
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FString		mOptionName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float		mOption;

public:
	const FString& GetOptionName()	const
	{
		return mOptionName;
	}

	float GetOption()	const
	{
		return mOption;
	}

public:
	void SetOptionName(const FString& OptionName)
	{
		mOptionName = OptionName;
	}

	void SetOption(float Option)
	{
		mOption = Option;
	}
};
