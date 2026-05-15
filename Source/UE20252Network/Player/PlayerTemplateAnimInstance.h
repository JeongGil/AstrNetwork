// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PlayerAnimInstance.h"
#include "PlayerTemplateAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class UE20252NETWORK_API UPlayerTemplateAnimInstance : public UPlayerAnimInstance
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TMap<FString, TObjectPtr<UAnimSequence>>	mAnimMap;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TMap<FString, TObjectPtr<UBlendSpace>>	mBlendSpaceMap;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TMap<FString, TObjectPtr<UAnimMontage>>	mSkillMap;

public:
	//virtual void NativeInitializeAnimation();
	// Native update override point. It is usually a good idea to simply gather data in this step and 
	virtual void NativeBeginPlay();
	// for the bulk of the work to be done in NativeThreadSafeUpdateAnimation.
	virtual void NativeUpdateAnimation(float DeltaSeconds);
	// Native thread safe update override point. Executed on a worker thread just prior to graph update 
	// for linked anim instances, only called when the hosting node(s) are relevant
	//virtual void NativeThreadSafeUpdateAnimation(float DeltaSeconds);
	// Native Post Evaluate override point
	//virtual void NativePostEvaluateAnimation();
	// Native Uninitialize override point
	//virtual void NativeUninitializeAnimation();

public:
	UFUNCTION()
	void AnimNotify_SkillCasting();

protected:
	virtual void MontageEndOverride(UAnimMontage* Montage, bool bInterrupted);
};
