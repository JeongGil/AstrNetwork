// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "Engine/DataAsset.h"
#include "ItemInfoTableAsset.generated.h"

/**
 * 
 */
UCLASS()
class UE20252NETWORK_API UItemInfoTableAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()


public:
	// TSoftObjectPtr : 에셋이나 오브젝트를 경로만 가지고 있다가 필요할 때 로드하는 포인터
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TSoftObjectPtr<UDataTable>	mTable;
};
