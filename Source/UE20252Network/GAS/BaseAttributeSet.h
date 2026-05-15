// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "AttributeSet.h"
#include "BaseAttributeSet.generated.h"

/**
 * 
 */
UCLASS()
class UE20252NETWORK_API UBaseAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attribute", meta = (AllowPrivateAccess = "true"))
	FGameplayAttributeData	Attack;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attribute", meta = (AllowPrivateAccess = "true"))
	FGameplayAttributeData	Defense;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attribute", meta = (AllowPrivateAccess = "true"))
	FGameplayAttributeData	HP;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attribute", meta = (AllowPrivateAccess = "true"))
	FGameplayAttributeData	HPMax;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attribute", meta = (AllowPrivateAccess = "true"))
	FGameplayAttributeData	MP;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attribute", meta = (AllowPrivateAccess = "true"))
	FGameplayAttributeData	MPMax;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attribute", meta = (AllowPrivateAccess = "true"))
	FGameplayAttributeData	WalkSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attribute", meta = (AllowPrivateAccess = "true"))
	FGameplayAttributeData	RunSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attribute", meta = (AllowPrivateAccess = "true"))
	FGameplayAttributeData	AttackDistance;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attribute", meta = (AllowPrivateAccess = "true"))
	FGameplayAttributeData	Gold;

public:
	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data);

public:
	virtual void CallbackHP(AActor* Instigator);

public:
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, Attack)
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, Defense)
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, HP)
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, HPMax)
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, MP)
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, MPMax)
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, WalkSpeed)
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, RunSpeed)
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, AttackDistance)
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, Gold)
};
