// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "PlayerEntryObject.generated.h"

/**
 *
 */
UCLASS()
class UE20252NETWORK_API UPlayerEntryObject : public UObject
{
	GENERATED_BODY()

public:
	UPlayerEntryObject();

	FString PlayerName;
};
