// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayEffect_ShinbiSkill2.h"

UGameplayEffect_ShinbiSkill2::UGameplayEffect_ShinbiSkill2()
{
	// 얼마나 지속될 것인지를 결정한다.
	// Instant : 즉시 동작.
	// HasDuration : 일정시간 동작.
	// Infinite : 무한하게 동작.
	DurationPolicy = EGameplayEffectDurationType::HasDuration;

	FSetByCallerFloat	Caller;

	Caller.DataTag = FGameplayTag::RequestGameplayTag(
		TEXT("Effect.Battle.Shinbi.Skill2Time"));

	DurationMagnitude = FGameplayEffectModifierMagnitude(Caller);

	// 실행될 시간.
	Period = 1.f;
	bExecutePeriodicEffectOnApplication = true;
}
