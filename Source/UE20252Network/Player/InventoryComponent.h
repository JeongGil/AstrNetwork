// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_TwoParams(FInventoryItemChange, class UItemObject*, int32);
DECLARE_MULTICAST_DELEGATE_TwoParams(FInventoryItemCountChange, int32, int32);
DECLARE_MULTICAST_DELEGATE_TwoParams(FInventoryItemEquipChange, int32, bool);
DECLARE_MULTICAST_DELEGATE_OneParam(FInventoryGoldChange, int32);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UE20252NETWORK_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UInventoryComponent();

protected:
	UPROPERTY(VisibleAnywhere, Category = "Inventory")
	TArray<TObjectPtr<class UItemObject>>	mItemList;

	UPROPERTY(VisibleAnywhere, Category = "Inventory")
	int32			mInventoryMaxCount = 30;

	UPROPERTY(VisibleAnywhere, Category = "Inventory")
	int32			mItemCount = 0;

	int32			mEquipWeaponIndex = -1;
	FActiveGameplayEffectHandle		mWeaponHandle;
	TObjectPtr<class UItemObject>	mEquipWeapon;

public:
	int32 GetInventoryMax()	const
	{
		return mInventoryMaxCount;
	}

	int32 GetItemCount()	const
	{
		return mItemCount;
	}

	class UItemObject* GetItem(int32 Index)
	{
		return mItemList[Index];
	}

public:
	FInventoryItemChange	mItemChange;
	FInventoryGoldChange	mGoldChange;
	FInventoryItemCountChange	mItemCountChange;
	FInventoryItemEquipChange	mItemEquipChange;


protected:
	virtual void BeginPlay() override;

public:
	virtual void InitializeComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	void AddItem(const FItemTableInfo* Info);
	void ChangeGold(int32 Gold);
	void ItemInfoLoadComplete();
	void UseItem(int32 Index);
	void SwapItem(int32 StartIndex, int32 ChangeIndex);
	void RemoveItem(int32 Index);
	bool CheckItem(int32 Index);

private:
	void UsePotion(int32 Index);
	void UseWeapon(int32 Index);
	void UseArmor(int32 Index);

	float GetItemAttack(int32 Index);
	void ApplyItemAttack(int32 Index);
	void RemoveItemAttack(int32 Index);
};
