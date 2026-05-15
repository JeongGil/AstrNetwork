// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "Abilities/GameplayAbility.h"
#include "GameplayAbility_Base.generated.h"

/**
 * 
 */
UCLASS()
class UE20252NETWORK_API UGameplayAbility_Base : public UGameplayAbility
{
	GENERATED_BODY()
	
public:
	UGameplayAbility_Base();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ability", meta = (AllowPrivateAccess = "true"))
	float		mMana;

	TSubclassOf<UGameplayEffect>	mManaCostClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ability", meta = (AllowPrivateAccess = "true"))
	float		mCoolDown;

	FGameplayTag	mCoolDownTag;

	TSubclassOf<UGameplayEffect>	mCoolDownClass;

	// mutable은 const 함수 안에서도 이 변수만은 수정할 수 있게 허용한다.
	mutable FGameplayTagContainer	mCoolDownTags;

	bool		mAbilityActive = false;

protected:
	virtual const FGameplayTagContainer* GetCooldownTags() const;

	virtual void ApplyCooldown(const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const;

	virtual bool CheckCost(const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const;

	virtual void ApplyCost(const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo) const;

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo, 
		const FGameplayAbilityActivationInfo ActivationInfo, 
		const FGameplayEventData* TriggerEventData);

public:
	const TArray<FAbilityTriggerData>& GetAbilityTriggers()	const
	{
		return AbilityTriggers;
	}
};
