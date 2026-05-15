// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbility_MonsterBase.h"

UGameplayAbility_MonsterBase::UGameplayAbility_MonsterBase()
{
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::ServerOnly;
}

void UGameplayAbility_MonsterBase::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}
