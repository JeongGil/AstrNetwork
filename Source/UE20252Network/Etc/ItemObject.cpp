// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemObject.h"

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
