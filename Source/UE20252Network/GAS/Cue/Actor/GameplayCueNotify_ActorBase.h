// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../../GameInfo.h"
#include "GameplayCueNotify_Actor.h"
#include "GameplayCueNotify_ActorBase.generated.h"

/**
 * 
 */
UCLASS()
class UE20252NETWORK_API AGameplayCueNotify_ActorBase : public AGameplayCueNotify_Actor
{
	GENERATED_BODY()

public:
	AGameplayCueNotify_ActorBase();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effect")
	TObjectPtr<UParticleSystem>		mParticle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effect")
	TObjectPtr<UNiagaraSystem>		mNiagara;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effect")
	TObjectPtr<USoundBase>			mSound;

	TObjectPtr<UParticleSystemComponent>	mParticleComponent;
	TObjectPtr<UNiagaraComponent>	mNiagaraComponent;
	TObjectPtr<UAudioComponent>	mAudioComponent;

public:
	bool OnActive_Implementation(AActor* Target,
		const FGameplayCueParameters& Parameters);

	bool OnRemove_Implementation(AActor* Target,
		const FGameplayCueParameters& Parameters);
};
