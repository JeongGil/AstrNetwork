// Fill out your copyright notice in the Description page of Project Settings.


#include "GameInfo.h"

// 로그 정의
DEFINE_LOG_CATEGORY(UELOG);

FRotator GetTargetRotator(const FVector& Target, const FVector& Self)
{
	return UKismetMathLibrary::FindLookAtRotation(Self, Target);
}

FRotator GetTargetRotatorYaw(FVector Target, FVector Self)
{
	Target.Z = 0.0;
	Self.Z = 0.0;

	return UKismetMathLibrary::FindLookAtRotation(Self, Target);
}

FString GetItemTypeName(EItemType ItemType)
{
	switch (ItemType)
	{
	case EItemType::Sword:
		return TEXT("검");
	case EItemType::Bow:
		return TEXT("활");
	case EItemType::Staff:
		return TEXT("지팡이");
	case EItemType::Gun:
		return TEXT("총");
	case EItemType::Armor:
		return TEXT("갑옷");
	case EItemType::Helmet:
		return TEXT("투구");
	case EItemType::Glove:
		return TEXT("장갑");
	case EItemType::Shoes:
		return TEXT("신발");
	case EItemType::Potion:
		return TEXT("물약");
	case EItemType::Scroll:
		return TEXT("주문서");
	}

	return TEXT("");
}

FString GetItemOptionTypeName(EItemOptionType ItemType)
{
	switch (ItemType)
	{
	case EItemOptionType::Attack:
		return TEXT("공격력");
	case EItemOptionType::Defense:
		return TEXT("방어력");
	case EItemOptionType::HPMax:
		return TEXT("최대체력");
	case EItemOptionType::MPMax:
		return TEXT("최대마나");
	case EItemOptionType::HPRecoveryPoint:
		return TEXT("체력회복");
	case EItemOptionType::HPRecoveryRate:
		return TEXT("체력회복비율");
	case EItemOptionType::MPRecoveryPoint:
		return TEXT("마나회복");
	case EItemOptionType::MPRecoveryRate:
		return TEXT("마나회복비율");
	case EItemOptionType::MoveSpeed:
		return TEXT("이동속도");
	case EItemOptionType::AttackSpeed:
		return TEXT("공격속도");
	case EItemOptionType::AttackDistance:
		return TEXT("공격거리");
	case EItemOptionType::CriticalRatio:
		return TEXT("치명타비율");
	case EItemOptionType::CriticalDamage:
		return TEXT("치명타피해");
	}

	return TEXT("");
}
