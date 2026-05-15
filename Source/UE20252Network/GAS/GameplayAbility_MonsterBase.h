// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GameplayAbility_MonsterBase.generated.h"

/**
 *
 */
UCLASS()
class UE20252NETWORK_API UGameplayAbility_MonsterBase : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UGameplayAbility_MonsterBase();

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

public:
	FORCEINLINE const TArray<FAbilityTriggerData>& GetAbilityTriggers() const
	{
		return AbilityTriggers;
	}
};
