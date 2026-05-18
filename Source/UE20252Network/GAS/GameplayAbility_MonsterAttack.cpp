// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbility_MonsterAttack.h"

#include "BaseAttributeSet.h"
#include "Effect/GameplayEffect_Damage.h"

UGameplayAbility_MonsterAttack::UGameplayAbility_MonsterAttack()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	FAbilityTriggerData TriggerData;
	TriggerData.TriggerTag = FGameplayTag::RequestGameplayTag(TEXT("Ability.Monster.Attack"));
	TriggerData.TriggerSource = EGameplayAbilityTriggerSource::GameplayEvent;

	AbilityTriggers.Add(TriggerData);
}

void UGameplayAbility_MonsterAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                                     const FGameplayAbilityActorInfo* ActorInfo,
                                                     const FGameplayAbilityActivationInfo ActivationInfo,
                                                     const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (!TriggerEventData || !TriggerEventData->Target)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		return;
	}

	// Hit 정보를 얻어온다.
	auto* HitData = (FGameplayAbilityTargetData_SingleTargetHit*)(TriggerEventData->TargetData.Data[0].Get());

	if (!HitData)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		return;
	}


	// 이 어빌리티를 발동시킨 Actor를 얻어온다.
	// AActor* SourceActor = GetAvatarActorFromActorInfo();

	// SourceActor의 AbilitySystemComponent를 얻어온다.
	UAbilitySystemComponent* SourceASC = GetAbilitySystemComponentFromActorInfo();

	// Source AttributeSet을 얻어온다. GetSet 함수는 const 포인터를 반환하는데
	// const 포인터일 경우 데이터를 변경할 수 없으므로 const_cast를 이용해서 풀어준다.
	/*UBaseAttributeSet* SourceAttr = const_cast<UBaseAttributeSet*>(SourceASC->GetSet<UBaseAttributeSet>());*/
	const auto* SourceAttr = SourceASC->GetSet<UBaseAttributeSet>();

	// 타겟을 얻어온다.
	AActor* TargetActor = HitData->HitResult.GetActor();

	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	if (!SourceASC || !TargetASC)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		return;
	}

	const auto* TargetAttr = TargetASC->GetSet<UBaseAttributeSet>();
	if (!TargetAttr)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		return;
	}

	// Damage 이펙트 발동.
	// FGameplayEffectContextHandle는 GameplayEffect가 어떤 상황에서 발생했는지에 대한
	// 부가적인 정보를 추가해주는 기능을 제공한다.
	FGameplayEffectContextHandle Context = MakeEffectContext(Handle, ActorInfo);

	Context.AddHitResult(HitData->HitResult);

	FGameplayEffectSpecHandle DamageSpec = MakeOutgoingGameplayEffectSpec(
		UGameplayEffect_Damage::StaticClass(), GetAbilityLevel());

	DamageSpec.Data->SetContext(Context);

	float Attack = SourceAttr->GetAttack();
	float Defense = TargetAttr->GetDefense();

	float Damage = Attack - Defense;
	Damage = FMath::Max(1.f, Damage);

	// GameplayEffect에 지정된 SetByCaller의 값을 지정한다.
	DamageSpec.Data->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(TEXT("Effect.Battle.Damage")), -Damage);

	// Target에게 Damage Effect가 발생하게 한다.
	FGameplayAbilityTargetDataHandle TargetData;

	// Damage Effect 대상을 추가할 수 있게 한다.
	auto* TargetArray = new FGameplayAbilityTargetData_ActorArray;
	TargetArray->TargetActorArray.Add(TargetActor);

	TargetData.Add(TargetArray);

	// GameplayEffect를 실행시킨다. 대상 Target들에게 모두 적용시킨다.
	ApplyGameplayEffectSpecToTarget(Handle, ActorInfo, ActivationInfo, DamageSpec, TargetData);

	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}
