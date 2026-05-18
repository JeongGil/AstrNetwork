// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "UObject/Object.h"
#include "ItemObject.generated.h"

/**
 *
 */
UCLASS(BlueprintType)
class UE20252NETWORK_API UItemObject : public UObject
{
	GENERATED_BODY()

public:
	UItemObject();

private:
	UPROPERTY(EditAnywhere, Replicated, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FString		mItemName;

	UPROPERTY(EditAnywhere, Replicated, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	EItemType	mItemType;

	UPROPERTY(EditAnywhere, Replicated, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	EItemWidgetLayerType	mWidgetLayeredType;

	UPROPERTY(EditAnywhere, Replicated, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	int32		mLevel;

	UPROPERTY(EditAnywhere, Replicated, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FString		mDesc;

	UPROPERTY(EditAnywhere, Replicated, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	int32		mPurchasePrice;

	UPROPERTY(EditAnywhere, Replicated, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	int32		mSellPrice;

	UPROPERTY(EditAnywhere, Replicated, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UTexture2D* mIconImage;

	UPROPERTY(EditAnywhere, Replicated, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UStaticMesh* mMesh;

	UPROPERTY(EditAnywhere, Replicated, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TArray<FItemOption>		mOptions;

	UPROPERTY(EditAnywhere, Replicated, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	int32					mUpgrade;

	UPROPERTY(EditAnywhere, Replicated, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	int32					mCount = 1;

public:
	EItemWidgetLayerType GetItemLayerType()	const
	{
		return mWidgetLayeredType;
	}

	FString GetItemName()	const
	{
		return mItemName;
	}

	EItemType GetItemType()	const
	{
		return mItemType;
	}

	int32 GetItemLevel()	const
	{
		return mLevel;
	}

	FString GetItemDesc()	const
	{
		return mDesc;
	}

	int32 GetItemPurchasePrice()	const
	{
		return mPurchasePrice;
	}

	int32 GetItemSellPrice()	const
	{
		return mSellPrice;
	}

	UTexture2D* GetItemIconImage()	const
	{
		return mIconImage;
	}

	UStaticMesh* GetItemMesh()	const
	{
		return mMesh;
	}

	float GetItemOption(EItemOptionType Type)	const
	{
		float	Value = 0.f;

		for (auto Option : mOptions)
		{
			if (Option.Type == Type)
				Value += Option.Option;
		}

		return Value;
	}

	int32 GetItemUpgrade(EItemOptionType Type)	const
	{
		return mUpgrade;
	}

	int32 GetItemCount()	const
	{
		return mCount;
	}

	void AddCount()
	{
		++mCount;
	}

	void AddCount(int32 Count)
	{
		mCount += Count;
	}

	int32 GetItemOptionCount()	const
	{
		return mOptions.Num();
	}

	FItemOption GetItemOption(int32 Index)
	{
		return mOptions[Index];
	}

	const TArray<FItemOption>& GetItemOptions()	const
	{
		return mOptions;
	}

public:
	void SetItemInfo(const FItemTableInfo* Info);

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual FORCEINLINE bool IsSupportedForNetworking() const override;
};
