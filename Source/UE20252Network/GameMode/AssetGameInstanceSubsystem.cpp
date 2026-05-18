// Fill out your copyright notice in the Description page of Project Settings.


#include "AssetGameInstanceSubsystem.h"
#include "../Monster/MonsterInfoTableAsset.h"
#include "../Monster/DropItemInfoTableAsset.h"
#include "../Etc/ItemInfoTableAsset.h"
#include "Engine/AssetManager.h"

void UAssetGameInstanceSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	GEngine->AddOnScreenDebugMessage(-1, 1000.f, FColor::Red, TEXT("Subsystem Initialize"));

	LoadMonsterData();

	LoadItemData();

	LoadDropItemData();
}

void UAssetGameInstanceSubsystem::Deinitialize()
{
	Super::Deinitialize();
}

void UAssetGameInstanceSubsystem::LoadMonsterData()
{
	UAssetManager& AssetMgr = UAssetManager::Get();

	FPrimaryAssetId AssetId(TEXT("MonsterInfoTableAsset"), TEXT("PDA_MonsterInfo"));

	// 비동기 로딩을 진행해준다. 로딩 명령을 내려놓고 로딩이 끝나면 지정된 함수가
	// 호출된다.
	AssetMgr.LoadPrimaryAsset(AssetId, TArray<FName>{},
	                          FStreamableDelegate::CreateUObject(
		                          this, &UAssetGameInstanceSubsystem::MonsterInfoLoadComplete, AssetId));
}

void UAssetGameInstanceSubsystem::MonsterInfoLoadComplete(FPrimaryAssetId LoadId)
{
	UAssetManager& AssetMgr = UAssetManager::Get();

	// 로딩된 오브젝트를 얻어온다.
	TObjectPtr<UObject> LoadObject = AssetMgr.GetPrimaryAssetObject(LoadId);

	TObjectPtr<UMonsterInfoTableAsset> DataAsset =
		Cast<UMonsterInfoTableAsset>(LoadObject);

	if (!DataAsset)
	{
		UE_LOG(UELOG, Warning, TEXT("MonsterInfo Load Failed"));
		return;
	}

	mMonsterInfoTable = DataAsset->mTable.LoadSynchronous();

	if (!mMonsterInfoTable)
	{
		UE_LOG(UELOG, Warning, TEXT("MonsterInfo DataTable Load Failed"));
		return;
	}

	UE_LOG(UELOG, Warning, TEXT("MonsterInfo Load Complete"));

	// 등록된 함수를 호출한다.
	if (mMonsterInfoLoadDelegate.IsBound())
		mMonsterInfoLoadDelegate.Broadcast();
}

const FMonsterInfo* UAssetGameInstanceSubsystem::FindMonsterInfo(const FName& Name) const
{
	if (!mMonsterInfoTable)
		return nullptr;

	return mMonsterInfoTable->FindRow<FMonsterInfo>(Name, TEXT("MonsterInfo"));
}

void UAssetGameInstanceSubsystem::LoadItemData()
{
	UAssetManager& AssetMgr = UAssetManager::Get();

	FPrimaryAssetId ItemAssetId(TEXT("ItemInfoTableAsset"), TEXT("PDA_ItemInfo"));

	// 비동기 로딩을 진행해준다. 로딩 명령을 내려놓고 로딩이 끝나면 지정된 함수가
	// 호출된다.
	AssetMgr.LoadPrimaryAsset(ItemAssetId, TArray<FName>{},
	                          FStreamableDelegate::CreateUObject(
		                          this, &UAssetGameInstanceSubsystem::ItemInfoLoadComplete, ItemAssetId));
}

void UAssetGameInstanceSubsystem::ItemInfoLoadComplete(FPrimaryAssetId LoadId)
{
	UAssetManager& AssetMgr = UAssetManager::Get();

	// 로딩된 오브젝트를 얻어온다.
	TObjectPtr<UObject> LoadObject = AssetMgr.GetPrimaryAssetObject(LoadId);

	TObjectPtr<UItemInfoTableAsset> DataAsset =
		Cast<UItemInfoTableAsset>(LoadObject);

	if (!DataAsset)
	{
		GEngine->AddOnScreenDebugMessage(-1, 100.f, FColor::Blue, TEXT("ItemInfo Load Failed"));
		UE_LOG(UELOG, Warning, TEXT("ItemInfo Load Failed"));
		return;
	}

	mItemInfoTable = DataAsset->mTable.LoadSynchronous();

	if (!mItemInfoTable)
	{
		GEngine->AddOnScreenDebugMessage(-1, 100.f, FColor::Blue, TEXT("ItemInfo DataTable Load Failed"));
		UE_LOG(UELOG, Warning, TEXT("ItemInfo DataTable Load Failed"));
		return;
	}

	GEngine->AddOnScreenDebugMessage(-1, 100.f, FColor::Blue, TEXT("ItemInfo Load Complete"));
	UE_LOG(UELOG, Warning, TEXT("ItemInfo Load Complete"));

	// 등록된 함수를 호출한다.
	if (mItemInfoLoadDelegate.IsBound())
		mItemInfoLoadDelegate.Broadcast();
}

const FItemTableInfo* UAssetGameInstanceSubsystem::FindItemInfo(const FName& Name) const
{
	if (!mItemInfoTable)
		return nullptr;

	return mItemInfoTable->FindRow<FItemTableInfo>(Name, TEXT("ItemInfo"));
}

void UAssetGameInstanceSubsystem::LoadDropItemData()
{
	UAssetManager& AssetMgr = UAssetManager::Get();

	FPrimaryAssetId ItemAssetId(TEXT("DropItemInfoTableAsset"), TEXT("PDA_DropItemInfo"));

	// 비동기 로딩을 진행해준다. 로딩 명령을 내려놓고 로딩이 끝나면 지정된 함수가
	// 호출된다.
	AssetMgr.LoadPrimaryAsset(ItemAssetId, TArray<FName>{},
	                          FStreamableDelegate::CreateUObject(
		                          this, &UAssetGameInstanceSubsystem::DropItemInfoLoadComplete, ItemAssetId));
}

void UAssetGameInstanceSubsystem::DropItemInfoLoadComplete(FPrimaryAssetId LoadId)
{
	UAssetManager& AssetMgr = UAssetManager::Get();

	// 로딩된 오브젝트를 얻어온다.
	TObjectPtr<UObject> LoadObject = AssetMgr.GetPrimaryAssetObject(LoadId);

	TObjectPtr<UDropItemInfoTableAsset> DataAsset =
		Cast<UDropItemInfoTableAsset>(LoadObject);

	if (!DataAsset)
	{
		UE_LOG(UELOG, Warning, TEXT("DropItemInfo Load Failed"));
		return;
	}

	mDropItemInfoTable = DataAsset->mTable.LoadSynchronous();

	if (!mDropItemInfoTable)
	{
		UE_LOG(UELOG, Warning, TEXT("DropItemInfo DataTable Load Failed"));
		return;
	}

	UE_LOG(UELOG, Warning, TEXT("DropItemInfo Load Complete"));

	// 등록된 함수를 호출한다.
	if (mDropItemInfoLoadDelegate.IsBound())
		mDropItemInfoLoadDelegate.Broadcast();
}

const FDropItemTableInfo* UAssetGameInstanceSubsystem::FindDropItemInfo(
	const FName& Name) const
{
	if (!mDropItemInfoTable)
		return nullptr;

	return mDropItemInfoTable->FindRow<FDropItemTableInfo>(Name, TEXT("DropItemInfo"));
}
