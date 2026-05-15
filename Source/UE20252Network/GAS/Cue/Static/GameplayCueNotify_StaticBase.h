// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../../GameInfo.h"
#include "GameplayCueNotify_Static.h"
#include "GameplayCueNotify_StaticBase.generated.h"

/**
 * 
 */
UCLASS()
class UE20252NETWORK_API UGameplayCueNotify_StaticBase : public UGameplayCueNotify_Static
{
	GENERATED_BODY()
	
public:
	UGameplayCueNotify_StaticBase();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effect")
	TObjectPtr<UParticleSystem>		mParticle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effect")
	TObjectPtr<UNiagaraSystem>		mNiagara;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effect")
	TObjectPtr<USoundBase>			mSound;

public:
	bool OnExecute_Implementation(AActor* Target,
		const FGameplayCueParameters& Parameters) const;

	bool OnActive_Implementation(AActor* Target,
		const FGameplayCueParameters& Parameters) const;

	bool WhileActive_Implementation(AActor* Target,
		const FGameplayCueParameters& Parameters) const;

	bool OnRemove_Implementation(AActor* Target,
		const FGameplayCueParameters& Parameters) const;
};
