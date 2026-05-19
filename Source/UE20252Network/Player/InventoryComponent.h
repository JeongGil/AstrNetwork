// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ActiveGameplayEffectHandle.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"

struct FItemTableInfo;
class UItemObject;

DECLARE_MULTICAST_DELEGATE_TwoParams(FInventoryItemChange, UItemObject*, int32);
DECLARE_MULTICAST_DELEGATE_TwoParams(FInventoryItemCountChange, int32, int32);
DECLARE_MULTICAST_DELEGATE_TwoParams(FInventoryItemEquipChange, int32, bool);
DECLARE_MULTICAST_DELEGATE_OneParam(FInventoryGoldChange, int32);

UCLASS(ClassGroup=(Custom), meta = (BlueprintSpawnableComponent))
class UE20252NETWORK_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UInventoryComponent();

protected:
	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_ItemList, Category = "Inventory")
	TArray<TObjectPtr<UItemObject>> mItemList;

	UPROPERTY(VisibleAnywhere, Category = "Inventory")
	int32 mInventoryMaxCount = 30;

	UPROPERTY(VisibleAnywhere, Replicated, Category = "Inventory")
	int32 mItemCount = 0;

	int32 mEquipWeaponIndex = -1;
	FActiveGameplayEffectHandle mWeaponHandle;
	TObjectPtr<UItemObject> mEquipWeapon;

	bool bConnectWidget = false;
	bool bIsPendingRefreshWidget = false;

public:
	int32 GetInventoryMax() const
	{
		return mInventoryMaxCount;
	}

	int32 GetItemCount() const
	{
		return mItemCount;
	}

	UItemObject* GetItem(int32 Index)
	{
		return mItemList[Index];
	}

public:
	FInventoryItemChange mItemChange;
	FInventoryGoldChange mGoldChange;
	FInventoryItemCountChange mItemCountChange;
	FInventoryItemEquipChange mItemEquipChange;

protected:
	virtual void BeginPlay() override;

public:
	virtual void InitializeComponent() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
	virtual bool ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	void AddItem(const FItemTableInfo* Info, FName ItemRowName);
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

public:
	UFUNCTION()
	void OnRep_ItemList();

	void LoadItem();
	void ConnectWidget();
	void RefreshInventory();
};
