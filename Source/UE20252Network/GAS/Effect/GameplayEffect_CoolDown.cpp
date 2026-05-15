// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayEffect_CoolDown.h"
#include "../BaseAttributeSet.h"

UGameplayEffect_CoolDown::UGameplayEffect_CoolDown()
{
	// 얼마나 지속될 것인지를 결정한다.
	// Instant : 즉시 동작.
	// HasDuration : 일정시간 동작.
	// Infinite : 무한하게 동작.
	DurationPolicy = EGameplayEffectDurationType::HasDuration;

	FSetByCallerFloat	Caller;

	Caller.DataTag = FGameplayTag::RequestGameplayTag(TEXT("Data.CoolDown"));

	DurationMagnitude = FGameplayEffectModifierMagnitude(Caller);
}
