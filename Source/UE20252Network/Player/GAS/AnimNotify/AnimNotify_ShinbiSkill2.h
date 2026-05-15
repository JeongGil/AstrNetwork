// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../../GameInfo.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_ShinbiSkill2.generated.h"

/**
 * 
 */
UCLASS()
class UE20252NETWORK_API UAnimNotify_ShinbiSkill2 : public UAnimNotify
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameplayEvent")
	FGameplayTag	mEventTag;

public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
		const FAnimNotifyEventReference& EventReference);
};
