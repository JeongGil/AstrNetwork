// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemObject.h"

#include "Net/UnrealNetwork.h"

UItemObject::UItemObject()
{
}

void UItemObject::SetItemInfo(const FItemTableInfo* Info)
{
	mItemName = Info->ItemName;
	mItemType = Info->ItemType;
	mWidgetLayeredType = Info->WidgetLayeredType;
	mLevel = Info->Level;
	mDesc = Info->Desc;
	mPurchasePrice = Info->PurchasePrice;
	mSellPrice = Info->SellPrice;
	mIconImage = Info->IconImage;
	mMesh = Info->Mesh;
	mOptions = Info->Options;
}

void UItemObject::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UItemObject, mItemName);
	DOREPLIFETIME(UItemObject, mItemType);
	DOREPLIFETIME(UItemObject, mWidgetLayeredType);
	DOREPLIFETIME(UItemObject, mLevel);
	DOREPLIFETIME(UItemObject, mDesc);
	DOREPLIFETIME(UItemObject, mPurchasePrice);
	DOREPLIFETIME(UItemObject, mSellPrice);
	DOREPLIFETIME(UItemObject, mIconImage);
	DOREPLIFETIME(UItemObject, mMesh);
	DOREPLIFETIME(UItemObject, mOptions);
	DOREPLIFETIME(UItemObject, mUpgrade);
	DOREPLIFETIME(UItemObject, mCount);
}

bool UItemObject::IsSupportedForNetworking() const
{
	return true;
}
