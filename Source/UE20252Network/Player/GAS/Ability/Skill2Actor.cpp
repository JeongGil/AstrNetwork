// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill2Actor.h"
#include "../../../GAS/Effect/GameplayEffect_Damage.h"
#include "../../../GAS/BaseAttributeSet.h"

ASkill2Actor::ASkill2Actor()
{
	PrimaryActorTick.bCanEverTick = true;

	mBody = CreateDefaultSubobject<USphereComponent>(TEXT("Body"));

	SetRootComponent(mBody);

	mBody->SetSphereRadius(300.f);

	mBody->SetCollisionProfileName(TEXT("PlayerAreaAttack"));
}

void ASkill2Actor::BeginPlay()
{
	Super::BeginPlay();	
}

void ASkill2Actor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	GetWorldTimerManager().ClearTimer(mFinishTimer);
	GetWorldTimerManager().ClearTimer(mDamageTimer);

	if (mASC && mAreaCueTag.IsValid())
	{
		mASC->RemoveGameplayCue(mAreaCueTag);
	}

	UE_LOG(UELOG, Warning, TEXT("Skill2 End"));

	Super::EndPlay(EndPlayReason);
}

void ASkill2Actor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASkill2Actor::InitSkill(UAbilitySystemComponent* ASC)
{
	mASC = ASC;

	mAreaCueTag = FGameplayTag::RequestGameplayTag(
		TEXT("GameplayCue.Battle.Shinbi.Skill2"));

	StartEffect();
}

void ASkill2Actor::StartEffect()
{
	if (!mASC)
	{
		Destroy();
		return;
	}

	if (mAreaCueTag.IsValid())
	{
		FGameplayCueParameters	CueParams;

		CueParams.Location = GetActorLocation();
		CueParams.Normal = FVector::UpVector;
		CueParams.Instigator = mASC->GetAvatarActor();

		UE_LOG(UELOG, Warning, TEXT("Location : %s"), *CueParams.Location.ToString());

		mASC->AddGameplayCue(mAreaCueTag, CueParams);
	}

	// 즉시 1회 데미지를 준다.
	ApplyAreaDamage();

	GetWorldTimerManager().SetTimer(mDamageTimer, this,
		&ASkill2Actor::ApplyAreaDamage, mDamageInterval, true);


	GetWorldTimerManager().SetTimer(mFinishTimer, this,
		&ASkill2Actor::FinishEffect, mDuration, false);
}

void ASkill2Actor::ApplyAreaDamage()
{
	if (!mASC)
	{
		Destroy();
		return;
	}

	TArray<AActor*>	OverlapActors;

	mBody->GetOverlappingActors(OverlapActors);

	const UBaseAttributeSet* Attr = Cast<UBaseAttributeSet>(mASC->GetAttributeSet(UBaseAttributeSet::StaticClass()));

	float Attack = Attr->GetAttack();

	for (AActor* Target : OverlapActors)
	{
		if (!Target)
			continue;

		else if (Target == mASC->GetAvatarActor())
			continue;

		UAbilitySystemComponent* TargetASC =
			UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Target);

		if (!TargetASC)
			continue;

		// Damage GameplayEffect 실행
		FGameplayEffectContextHandle	Context = mASC->MakeEffectContext();
		Context.AddSourceObject(this);

		FGameplayEffectSpecHandle	SpecHandle =
			mASC->MakeOutgoingSpec(UGameplayEffect_Damage::StaticClass(), 1.f,
				Context);

		if (!SpecHandle.IsValid())
			continue;

		const UBaseAttributeSet* TargetAttr = Cast<UBaseAttributeSet>(TargetASC->GetAttributeSet(UBaseAttributeSet::StaticClass()));;

		float	Dmg = Attack - TargetAttr->GetDefense();

		if (Dmg < 1.f)
			Dmg = 1.f;

		UE_LOG(UELOG, Warning, TEXT("Damage : %.2f"), Dmg);

		SpecHandle.Data->SetSetByCallerMagnitude(
			FGameplayTag::RequestGameplayTag(TEXT("Effect.Battle.Damage")), -Dmg);

		mASC->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data, TargetASC);
	}
}

void ASkill2Actor::FinishEffect()
{
	if (mASC && mAreaCueTag.IsValid())
	{
		mASC->RemoveGameplayCue(mAreaCueTag);
	}

	if (mSkillFinishCallback.IsBound())
		mSkillFinishCallback.Broadcast();

	Destroy();
}
