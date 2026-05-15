// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbility_Attack.h"
#include "BaseAttributeSet.h"
#include "Effect/GameplayEffect_Damage.h"

UGameplayAbility_Attack::UGameplayAbility_Attack()
{
	// NonInstanced : 즉발형에 적합. 객체를 만들지 않고 CDO로 동작시킨다.
	// InstancedPerActor : 이 어빌리티를 사용하는 객체마다 1개씩 만들어진다.
	// InstancedPerExecution : 이 어빌리티가 발동될 때마다 매번 객체가 생성된다.
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	// 어빌리티가 어떤 신호로 자동 발동될지 정의하는 데이터이다.
	FAbilityTriggerData	TriggerData;

	TriggerData.TriggerTag = FGameplayTag::RequestGameplayTag(TEXT("Ability.Attack"));
	TriggerData.TriggerSource = EGameplayAbilityTriggerSource::GameplayEvent;

	AbilityTriggers.Add(TriggerData);
}

// 어빌리티가 실제로 발동됐을 때 실행되는 함수.
// Handle : ASC에 등록된 어빌리티 스펙을 식별하는 핸들이다.
// ActorInfo : 이 어빌리티를 누가 사용하고 있는지에 대한 정보들이다.
// ActivationInfo : 이 어빌리티가 어떤 방식으로 활성화 되었는지에 대한 상태 정보.
// TriggerEventData : 이 어빌리티가 이벤트로 발동됐을 때 추가로 전달되는 데이터.
void UGameplayAbility_Attack::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	UE_LOG(UELOG, Warning, TEXT("Ability Attack"));

	if (!mAbilityActive || !TriggerEventData || !TriggerEventData->Target)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		return;
	}

	// Hit 정보를 얻어온다.
	FGameplayAbilityTargetData_SingleTargetHit* HitData =
		(FGameplayAbilityTargetData_SingleTargetHit*)(TriggerEventData->TargetData.Data[0].Get());

	if (!HitData)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		return;
	}


	// 이 어빌리티를 발동시킨 Actor를 얻어온다.
	AActor* SourceActor = GetAvatarActorFromActorInfo();

	// SourceActor의 AbilitySystemComponent를 얻어온다.
	UAbilitySystemComponent* SourceASC = GetAbilitySystemComponentFromActorInfo();

	// Source AttributeSet을 얻어온다. GetSet 함수는 const 포인터를 반환하는데
	// const 포인터일 경우 데이터를 변경할 수 없으므로 const_cast를 이용해서 풀어준다.
	/*UBaseAttributeSet* SourceAttr = const_cast<UBaseAttributeSet*>(SourceASC->GetSet<UBaseAttributeSet>());*/
	const UBaseAttributeSet* SourceAttr = SourceASC->GetSet<UBaseAttributeSet>();

	// 타겟을 얻어온다.
	AActor* TargetActor = HitData->HitResult.GetActor();

	UAbilitySystemComponent* TargetASC = 
		UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);

	if (!SourceASC || !TargetASC)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		return;
	}

	const UBaseAttributeSet* TargetAttr = TargetASC->GetSet<UBaseAttributeSet>();

	if (!TargetAttr)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		return;
	}

	// Damage 이펙트 발동.
	// FGameplayEffectContextHandle는 GameplayEffect가 어떤 상황에서 발생했는지에 대한
	// 부가적인 정보를 추가해주는 기능을 제공한다.
	FGameplayEffectContextHandle	Context = MakeEffectContext(Handle, ActorInfo);

	Context.AddHitResult(HitData->HitResult);


	FGameplayEffectSpecHandle	DamageSpec = MakeOutgoingGameplayEffectSpec(
		UGameplayEffect_Damage::StaticClass(), GetAbilityLevel());

	DamageSpec.Data->SetContext(Context);

	float	Attack = SourceAttr->GetAttack();
	float	Defense = TargetAttr->GetDefense();

	float	Damage = Attack - Defense;
	Damage = FMath::Max(1.f, Damage);

	// GameplayEffect에 지정된 SetByCaller의 값을 지정한다.
	DamageSpec.Data->SetSetByCallerMagnitude(
		FGameplayTag::RequestGameplayTag(TEXT("Effect.Battle.Damage")), -Damage);

	// Target에게 Damage Effect가 발생하게 한다.
	FGameplayAbilityTargetDataHandle	TargetData;

	// Damage Effect 대상을 추가할 수 있게 한다.
	FGameplayAbilityTargetData_ActorArray* TargetArray =
		new FGameplayAbilityTargetData_ActorArray;

	TargetArray->TargetActorArray.Add(TargetActor);

	TargetData.Add(TargetArray);

	// GameplayEffect를 실행시킨다. 대상 Target들에게 모두 적용시킨다.
	ApplyGameplayEffectSpecToTarget(Handle, ActorInfo, ActivationInfo,
		DamageSpec, TargetData);



	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}
