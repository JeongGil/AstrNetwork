// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../../GameInfo.h"
#include "GameFramework/Actor.h"
#include "Skill2Actor.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnShinbiSkill2Finish);

UCLASS()
class UE20252NETWORK_API ASkill2Actor : public AActor
{
	GENERATED_BODY()
	
public:	
	ASkill2Actor();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USphereComponent> mBody;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAbilitySystemComponent> mASC;

	UPROPERTY(EditDefaultsOnly)
	FGameplayTag	mAreaCueTag;

	UPROPERTY(EditDefaultsOnly)
	float			mDuration = 5.f;

	UPROPERTY(EditDefaultsOnly)
	float			mDamageInterval = 1.f;

	FTimerHandle	mFinishTimer;
	FTimerHandle	mDamageTimer;

public:
	FOnShinbiSkill2Finish	mSkillFinishCallback;

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason);

public:	
	virtual void Tick(float DeltaTime) override;

public:
	void InitSkill(UAbilitySystemComponent* ASC);

private:
	void StartEffect();
	void ApplyAreaDamage();
	void FinishEffect();
};
