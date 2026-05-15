// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayAbility_MonsterBase.h"
#include "GameplayAbility_MonsterAttack.generated.h"

/**
 *
 */
UCLASS()
class UE20252NETWORK_API UGameplayAbility_MonsterAttack : public UGameplayAbility_MonsterBase
{
	GENERATED_BODY()

public:
	UGameplayAbility_MonsterAttack();

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
};
