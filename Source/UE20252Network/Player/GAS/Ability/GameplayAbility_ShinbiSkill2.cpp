// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbility_ShinbiSkill2.h"
#include "../../../GAS/BaseAttributeSet.h"
#include "../../../GAS/Effect/GameplayEffect_Damage.h"
#include "../../../GAS/Effect/GameplayEffect_CoolDown.h"
#include "Skill2Actor.h"

UGameplayAbility_ShinbiSkill2::UGameplayAbility_ShinbiSkill2()
{
	mCoolDown = 5.f;

	mCoolDownTag = FGameplayTag::RequestGameplayTag(
		TEXT("Effect.Skill.CoolDown2"));

	static ConstructorHelpers::FObjectFinder<UAnimMontage>	MontageAsset(TEXT("/Script/Engine.AnimMontage'/Game/Player/Shinbi/AM_Shinbi_Skill2_Template.AM_Shinbi_Skill2_Template'"));

	if (MontageAsset.Succeeded())
		mMontage = MontageAsset.Object;

	// NonInstanced : 즉발형에 적합. 객체를 만들지 않고 CDO로 동작시킨다.
	// InstancedPerActor : 이 어빌리티를 사용하는 객체마다 1개씩 만들어진다.
	// InstancedPerExecution : 이 어빌리티가 발동될 때마다 매번 객체가 생성된다.
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	
	//// 어빌리티가 어떤 신호로 자동 발동될지 정의하는 데이터이다.
	FAbilityTriggerData	TriggerData;

	TriggerData.TriggerTag = FGameplayTag::RequestGameplayTag(
		TEXT("Ability.Shinbi.Skill2"));
	TriggerData.TriggerSource = EGameplayAbilityTriggerSource::GameplayEvent;

	AbilityTriggers.Add(TriggerData);

	// Ability가 활성화 되는것을 막는 태그를 부여한다.
	ActivationBlockedTags.AddTag(FGameplayTag::RequestGameplayTag(TEXT("Effect.Skill.CoolDown2")));
}

// 어빌리티가 실제로 발동됐을 때 실행되는 함수.
// Handle : ASC에 등록된 어빌리티 스펙을 식별하는 핸들이다.
// ActorInfo : 이 어빌리티를 누가 사용하고 있는지에 대한 정보들이다.
// ActivationInfo : 이 어빌리티가 어떤 방식으로 활성화 되었는지에 대한 상태 정보.
// TriggerEventData : 이 어빌리티가 이벤트로 발동됐을 때 추가로 전달되는 데이터.
void UGameplayAbility_ShinbiSkill2::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (!mAbilityActive || !ActorInfo->AvatarActor.IsValid())
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		return;
	}

	mEventTag = FGameplayTag::RequestGameplayTag(TEXT("Event.Shinbi.Skill2"));

	UAbilityTask_WaitGameplayEvent* WaitEventTask =
		UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, mEventTag);

	if (WaitEventTask)
	{
		WaitEventTask->EventReceived.AddDynamic(this, 
			&UGameplayAbility_ShinbiSkill2::WaitEventReceive);

		WaitEventTask->ReadyForActivation();
	}

	// 가지고 있는 스킬 몽타주를 재생한다. AbilityTask_PlayMontageAndWait를 생성한다.
	UAbilityTask_PlayMontageAndWait* MontageTask =
		UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
			this, NAME_None, mMontage, 1.f);

	/*MontageTask->OnCompleted.AddDynamic(this,
		&UGameplayAbility_ShinbiSkill2::MontageComplete);*/

	MontageTask->ReadyForActivation();

	// Hit 정보를 얻어온다.
	//FGameplayAbilityTargetData_SingleTargetHit* HitData =
	//	(FGameplayAbilityTargetData_SingleTargetHit*)(TriggerEventData->TargetData.Data[0].Get());

	//if (!HitData)
	//{
	//	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
	//	return;
	//}


	//// 이 어빌리티를 발동시킨 Actor를 얻어온다.
	//AActor* SourceActor = GetAvatarActorFromActorInfo();

	//// SourceActor의 AbilitySystemComponent를 얻어온다.
	//UAbilitySystemComponent* SourceASC = GetAbilitySystemComponentFromActorInfo();

	//// Source AttributeSet을 얻어온다. GetSet 함수는 const 포인터를 반환하는데
	//// const 포인터일 경우 데이터를 변경할 수 없으므로 const_cast를 이용해서 풀어준다.
	///*UBaseAttributeSet* SourceAttr = const_cast<UBaseAttributeSet*>(SourceASC->GetSet<UBaseAttributeSet>());*/
	//const UBaseAttributeSet* SourceAttr = SourceASC->GetSet<UBaseAttributeSet>();

	//// 타겟을 얻어온다.
	//AActor* TargetActor = HitData->HitResult.GetActor();

	//UAbilitySystemComponent* TargetASC =
	//	UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);

	//if (!SourceASC || !TargetASC)
	//{
	//	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
	//	return;
	//}

	//const UBaseAttributeSet* TargetAttr = TargetASC->GetSet<UBaseAttributeSet>();

	//if (!TargetAttr)
	//{
	//	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
	//	return;
	//}

	//// Damage 이펙트 발동.
	//// FGameplayEffectContextHandle는 GameplayEffect가 어떤 상황에서 발생했는지에 대한
	//// 부가적인 정보를 추가해주는 기능을 제공한다.
	//FGameplayEffectContextHandle	Context = MakeEffectContext(Handle, ActorInfo);

	//Context.AddHitResult(HitData->HitResult);


	//FGameplayEffectSpecHandle	DamageSpec = MakeOutgoingGameplayEffectSpec(
	//	UGameplayEffect_Damage::StaticClass(), GetAbilityLevel());

	//DamageSpec.Data->SetContext(Context);

	//float	Attack = SourceAttr->GetAttack();
	//float	Defense = TargetAttr->GetDefense();

	//float	Damage = Attack - Defense;
	//Damage = FMath::Max(1.f, Damage);

	//// GameplayEffect에 지정된 SetByCaller의 값을 지정한다.
	//DamageSpec.Data->SetSetByCallerMagnitude(
	//	FGameplayTag::RequestGameplayTag(TEXT("Effect.Battle.Damage")), -Damage);

	//// Target에게 Damage Effect가 발생하게 한다.
	//FGameplayAbilityTargetDataHandle	TargetData;

	//// Damage Effect 대상을 추가할 수 있게 한다.
	//FGameplayAbilityTargetData_ActorArray* TargetArray =
	//	new FGameplayAbilityTargetData_ActorArray;

	//TargetArray->TargetActorArray.Add(TargetActor);

	//TargetData.Add(TargetArray);

	//// GameplayEffect를 실행시킨다. 대상 Target들에게 모두 적용시킨다.
	//ApplyGameplayEffectSpecToTarget(Handle, ActorInfo, ActivationInfo,
	//	DamageSpec, TargetData);



	//EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}

void UGameplayAbility_ShinbiSkill2::MontageComplete()
{
	/*EndAbility(GetCurrentAbilitySpecHandle(),
		GetCurrentActorInfo(),
		GetCurrentActivationInfo(),
		true, false);*/
}

void UGameplayAbility_ShinbiSkill2::WaitEventReceive(FGameplayEventData Payload)
{
	AActor* AvatarActor = GetAvatarActorFromActorInfo();

	if (!AvatarActor)
		return;

	UWorld* World = AvatarActor->GetWorld();

	if (!World)
		return;

	UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();

	if (!ASC)
		return;

	// 이펙트를 스폰시킨다.
	const FGameplayAbilityActorInfo* ActorInfo = GetCurrentActorInfo();

	APlayerController* PlayerController = ActorInfo->PlayerController.Get();

	FHitResult	Hit;

	bool Collision = PlayerController->GetHitResultUnderCursor(
		ECollisionChannel::ECC_GameTraceChannel5, true, Hit);

	if (!Collision)
	{
		EndAbility(GetCurrentAbilitySpecHandle(),
			GetCurrentActorInfo(),
			GetCurrentActivationInfo(),
			true, false);
		return;
	}

	FActorSpawnParameters	SpawnParam;
	SpawnParam.SpawnCollisionHandlingOverride =
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	ASkill2Actor* Skill2 = World->SpawnActor<ASkill2Actor>(Hit.ImpactPoint,
		FRotator::ZeroRotator, SpawnParam);

	Skill2->mSkillFinishCallback.AddUObject(this, 
		&UGameplayAbility_ShinbiSkill2::Skill2Finish);

	Skill2->InitSkill(ASC);
}

void UGameplayAbility_ShinbiSkill2::Skill2Finish()
{
	EndAbility(GetCurrentAbilitySpecHandle(),
		GetCurrentActorInfo(),
		GetCurrentActivationInfo(),
		true, false);
}
