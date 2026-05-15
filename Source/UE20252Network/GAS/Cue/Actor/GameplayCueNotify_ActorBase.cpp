// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayCueNotify_ActorBase.h"

AGameplayCueNotify_ActorBase::AGameplayCueNotify_ActorBase()
{
}

bool AGameplayCueNotify_ActorBase::OnActive_Implementation(AActor* Target,
	const FGameplayCueParameters& Parameters)
{
	if (IsValid(mParticle))
	{
		mParticleComponent = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),
			mParticle, Parameters.Location);
	}

	if (IsValid(mNiagara))
	{
		mNiagaraComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(),
			mNiagara, Parameters.Location);
	}

	if (IsValid(mSound))
	{
		mAudioComponent = UGameplayStatics::SpawnSoundAtLocation(GetWorld(), mSound,
			Parameters.Location);
	}

	return true;
}

bool AGameplayCueNotify_ActorBase::OnRemove_Implementation(AActor* Target,
	const FGameplayCueParameters& Parameters)
{
	if (IsValid(mParticleComponent))
	{
		mParticleComponent->DestroyComponent();
		mParticleComponent = nullptr;
	}

	if (IsValid(mNiagaraComponent))
	{
		mNiagaraComponent->DestroyComponent();
		mNiagaraComponent = nullptr;
	}

	if (IsValid(mAudioComponent))
	{
		mAudioComponent->DestroyComponent();
		mAudioComponent = nullptr;
	}

	return true;
}
