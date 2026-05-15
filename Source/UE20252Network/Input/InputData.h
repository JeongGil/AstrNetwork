// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "UObject/Object.h"
#include "InputData.generated.h"

UCLASS()
class UE20252NETWORK_API UInputData : public UObject
{
	GENERATED_BODY()

public:
	UInputData();

public:
	TObjectPtr<UInputMappingContext>	mContext;

protected:
	TMap<FString, TObjectPtr<UInputAction>>	mActions;

public:
	TObjectPtr<UInputAction> FindAction(const FString& Name)	const;
};

UCLASS()
class UE20252NETWORK_API UDefaultInputData : public UInputData
{
	GENERATED_BODY()
	
public:
	UDefaultInputData();
};

UCLASS()
class UE20252NETWORK_API USelectInputData : public UInputData
{
	GENERATED_BODY()

public:
	USelectInputData();
};
