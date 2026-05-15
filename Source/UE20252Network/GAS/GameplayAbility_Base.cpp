// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbility_Base.h"
#include "BaseAttributeSet.h"
#include "Effect/GameplayEffect_ManaCost.h"
#include "../Player/GAS/PlayerAbilitySystemComponent.h"
#include "Effect/GameplayEffect_CoolDown.h"

UGameplayAbility_Base::UGameplayAbility_Base()
{
	mCoolDownClass = UGameplayEffect_CoolDown::StaticClass();
	mManaCostClass = UGameplayEffect_ManaCost::StaticClass();
}

// Ability가 내부에서 CoolDown Check를 할 때 이 함수를 이용해서 CoolDown Tag를
// 얻어간다. 그렇기 때문에 우리가 지정한 Tag를 함께 반환해주기 위해서
// 이 함수를 재정의하고 TagContainer에 우리꺼 CoolDownTag를 추가하여 반환하게 하는것이다.
const FGameplayTagContainer* UGameplayAbility_Base::GetCooldownTags() const
{
	mCoolDownTags.Reset();

	// 부모함수를 호출해서 TagContainer를 얻어온다.
	const FGameplayTagContainer* ParentTags = Super::GetCooldownTags();

	if (ParentTags)
	{
		mCoolDownTags.AppendTags(*ParentTags);
	}

	if (mCoolDownTag.IsValid())
	{
		mCoolDownTags.AddTag(mCoolDownTag);
	}

	return &mCoolDownTags;
}

void UGameplayAbility_Base::ApplyCooldown(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo)	const
{
	if (!mCoolDownClass || !ActorInfo)
		return;

	UAbilitySystemComponent* ASC = ActorInfo->AbilitySystemComponent.Get();

	if (!ASC)
		return;

	FGameplayEffectSpecHandle	SpecHandle = MakeOutgoingGameplayEffectSpec(
		mCoolDownClass, GetAbilityLevel(Handle, ActorInfo));

	if (!SpecHandle.IsValid())
		return;

	// 쿨타임 감소는 여기에서 지정.
	float	CoolDown = mCoolDown;

	// GameplayEffect에 지정된 SetByCaller의 값을 지정한다.
	SpecHandle.Data->SetSetByCallerMagnitude(
		FGameplayTag::RequestGameplayTag(TEXT("Data.CoolDown")), CoolDown);

	// CoolDown Tag가 유효한지.
	if (mCoolDownTag.IsValid())
	{
		SpecHandle.Data->DynamicGrantedTags.AddTag(mCoolDownTag);
	}

	ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data);
}

bool UGameplayAbility_Base::CheckCost(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	if (!ActorInfo || !Super::CheckCost(Handle, ActorInfo, OptionalRelevantTags))
		return false;

	UAbilitySystemComponent* ASC = ActorInfo->AbilitySystemComponent.Get();

	if (!ASC)
		return false;

	// 현재 마나를 얻어온다.
	float	CurrentMana = ASC->GetNumericAttribute(UBaseAttributeSet::GetMPAttribute());

	// 스킬 사용시 필요 마나를 얻어온다.
	float	Mana = 0.f;

	UPlayerAbilitySystemComponent* PlayerASC =
		Cast<UPlayerAbilitySystemComponent>(ASC);

	if (IsValid(PlayerASC))
		Mana = PlayerASC->GetSkillMana();

	if (CurrentMana < Mana)
		return false;

	return true;
}

void UGameplayAbility_Base::ApplyCost(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo) const
{
	UAbilitySystemComponent* ASC = ActorInfo->AbilitySystemComponent.Get();

	if (!ASC)
		return;

	UPlayerAbilitySystemComponent* PlayerASC =
		Cast<UPlayerAbilitySystemComponent>(ASC);

	// Source AttributeSet을 얻어온다. GetSet 함수는 const 포인터를 반환하는데
	// const 포인터일 경우 데이터를 변경할 수 없으므로 const_cast를 이용해서 풀어준다.
	/*UBaseAttributeSet* SourceAttr = const_cast<UBaseAttributeSet*>(SourceASC->GetSet<UBaseAttributeSet>());*/
	const UBaseAttributeSet* SourceAttr = ASC->GetSet<UBaseAttributeSet>();

	float	Mana = 0.f;

	if (IsValid(PlayerASC))
		Mana = PlayerASC->GetSkillMana();

	// 마나를 소모시킨다.
	FGameplayEffectSpecHandle	ManaSpec = MakeOutgoingGameplayEffectSpec(
		UGameplayEffect_ManaCost::StaticClass(), GetAbilityLevel());

	// GameplayEffect에 지정된 SetByCaller의 값을 지정한다.
	ManaSpec.Data->SetSetByCallerMagnitude(
		FGameplayTag::RequestGameplayTag(TEXT("Effect.Mana")), -Mana);

	// GameplayEffect를 실행시킨다.
	ASC->ApplyGameplayEffectSpecToSelf(*ManaSpec.Data);
}

// 어빌리티가 실제로 발동됐을 때 실행되는 함수.
// Handle : ASC에 등록된 어빌리티 스펙을 식별하는 핸들이다.
// ActorInfo : 이 어빌리티를 누가 사용하고 있는지에 대한 정보들이다.
// ActivationInfo : 이 어빌리티가 어떤 방식으로 활성화 되었는지에 대한 상태 정보.
// TriggerEventData : 이 어빌리티가 이벤트로 발동됐을 때 추가로 전달되는 데이터.
void UGameplayAbility_Base::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	UE_LOG(UELOG, Warning, TEXT("UGameplayAbility_Base::ActivateAbility Start"));

	if (!ActorInfo || !ActorInfo->AvatarActor.IsValid())
	{
		UE_LOG(UELOG, Warning, TEXT("ActorInfo Failed"));

		mAbilityActive = false;
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		return;
	}

	UE_LOG(UELOG, Warning, TEXT("CommitAbility Start"));

	// 비용 적용 및 쿨타임 체크 혹은 적용
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		mAbilityActive = false;
		UE_LOG(UELOG, Warning, TEXT("CommitAbility Failed"));
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		return;
	}

	UE_LOG(UELOG, Warning, TEXT("CommitAbility End"));

	mAbilityActive = true;

	// 이 어빌리티를 발동시킨 Actor를 얻어온다.
	//AActor* SourceActor = GetAvatarActorFromActorInfo();

	//// SourceActor의 AbilitySystemComponent를 얻어온다.
	//UAbilitySystemComponent* SourceASC = GetAbilitySystemComponentFromActorInfo();

	//UPlayerAbilitySystemComponent* PlayerASC =
	//	Cast<UPlayerAbilitySystemComponent>(SourceASC);

	//// Source AttributeSet을 얻어온다. GetSet 함수는 const 포인터를 반환하는데
	//// const 포인터일 경우 데이터를 변경할 수 없으므로 const_cast를 이용해서 풀어준다.
	///*UBaseAttributeSet* SourceAttr = const_cast<UBaseAttributeSet*>(SourceASC->GetSet<UBaseAttributeSet>());*/
	//const UBaseAttributeSet* SourceAttr = SourceASC->GetSet<UBaseAttributeSet>();

	//if (IsValid(PlayerASC))
	//	mMana = PlayerASC->GetSkillMana();

	//else
	//	mMana = 0.f;

	//UE_LOG(UELOG, Warning, TEXT("Mana : %.2f"), mMana);

	//// 마나 소모가 있을 경우
	//if (mMana > 0.f)
	//{
	//	if (SourceAttr && SourceAttr->GetMP() < mMana)
	//	{
	//		UE_LOG(UELOG, Warning, TEXT("Mana가 부족합니다."));
	//		mAbilityActive = false;
	//		CancelAbility(Handle, ActorInfo, ActivationInfo, true);
	//		return;
	//	}
	//}

	//if (mCoolDown > 0.f)
	//{

	//}


	//// 마나를 소모시킨다.
	//FGameplayEffectSpecHandle	ManaSpec = MakeOutgoingGameplayEffectSpec(
	//	UGameplayEffect_ManaCost::StaticClass(), GetAbilityLevel());

	//// GameplayEffect에 지정된 SetByCaller의 값을 지정한다.
	//ManaSpec.Data->SetSetByCallerMagnitude(
	//	FGameplayTag::RequestGameplayTag(TEXT("Effect.Mana")), -mMana);

	//// GameplayEffect를 실행시킨다.
	//SourceASC->ApplyGameplayEffectSpecToSelf(*ManaSpec.Data);
}
