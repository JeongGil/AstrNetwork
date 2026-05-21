// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "AssetGameInstanceSubsystem.generated.h"

// 몬스터정보 로딩 완료 알림용 델리게이트
DECLARE_MULTICAST_DELEGATE(FOnMonsterDataLoad);

/**
 *
 */
UCLASS()
class UE20252NETWORK_API UAssetGameInstanceSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

protected:
	//TMap<FString, TObjectPtr<UDataTable>>	mTableMap;
	UPROPERTY()
	TObjectPtr<UDataTable>	mMonsterInfoTable;

	UPROPERTY()
	TObjectPtr<UDataTable>	mItemInfoTable;

	UPROPERTY()
	TObjectPtr<UDataTable>	mDropItemInfoTable;

	bool bMonsterInfoLoaded = false;
	bool bItemInfoLoaded = false;
	bool bDropItemInfoLoaded = false;

public:
	FOnMonsterDataLoad	mMonsterInfoLoadDelegate;
	FOnMonsterDataLoad	mItemInfoLoadDelegate;
	FOnMonsterDataLoad	mDropItemInfoLoadDelegate;

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection);
	virtual void Deinitialize();

	[[nodiscard]] bool IsMonsterInfoLoaded() const
	{
		return bMonsterInfoLoaded;
	}

	[[nodiscard]] bool IsItemInfoLoaded() const
	{
		return bItemInfoLoaded;
	}

	[[nodiscard]] bool IsDropItemInfoLoaded() const
	{
		return bDropItemInfoLoaded;
	}

	void LoadMonsterData();

	UFUNCTION()
	void MonsterInfoLoadComplete(FPrimaryAssetId LoadId);

	const FMonsterInfo* FindMonsterInfo(const FName& Name)	const;

public:
	void LoadItemData();

	UFUNCTION()
	void ItemInfoLoadComplete(FPrimaryAssetId LoadId);

	const FItemTableInfo* FindItemInfo(const FName& Name)	const;

public:
	void LoadDropItemData();

	UFUNCTION()
	void DropItemInfoLoadComplete(FPrimaryAssetId LoadId);

	const FDropItemTableInfo* FindDropItemInfo(const FName& Name)	const;
};
