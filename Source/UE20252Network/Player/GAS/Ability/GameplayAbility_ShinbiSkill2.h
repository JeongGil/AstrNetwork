// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../../GAS/GameplayAbility_Base.h"
#include "GameplayAbility_ShinbiSkill2.generated.h"

UCLASS()
class UE20252NETWORK_API UGameplayAbility_ShinbiSkill2 : public UGameplayAbility_Base
{
	GENERATED_BODY()

public:
	UGameplayAbility_ShinbiSkill2();

protected:
	// EditDefaultsOnly : 블루프린트 클래스에서 기본값 수정은 가능하지만 월드에 배치된 인스턴
	// 스 에서는 수정할 수 없게 한다.
	UPROPERTY(EditDefaultsOnly, Category = "Skill2")
	TObjectPtr<UAnimMontage>	mMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Skill2")
	FGameplayTag				mEventTag;

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData);

public:
	UFUNCTION()
	void MontageComplete();

	UFUNCTION()
	void WaitEventReceive(FGameplayEventData Payload);

	UFUNCTION()
	void Skill2Finish();
};
