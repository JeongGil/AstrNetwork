// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../GameInfo.h"
#include "Animation/AnimInstance.h"
#include "MonsterGASAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class UE20252NETWORK_API UMonsterGASAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	EMonsterNormalAnim	mAnimType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TMap<FString, TObjectPtr<UAnimSequence>>	mAnimMap;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TMap<FString, TObjectPtr<UBlendSpace>>	mBlendSpaceMap;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TMap<FString, TObjectPtr<UAnimMontage>>	mMontageMap;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float		mHitAlpha = 0.f;

public:
	void SetAnim(EMonsterNormalAnim Anim)
	{
		mAnimType = Anim;
	}

	void Hit()
	{
		mHitAlpha = 0.5f;
	}

public:
	virtual void NativeBeginPlay();
	virtual void NativeUpdateAnimation(float DeltaSeconds);

public:
	// 노티파이 함수 규칙 : AnimNotify_노티파이이름() 으로 함수를 만든다.
	UFUNCTION()
	void AnimNotify_Attack();

	UFUNCTION()
	void AnimNotify_AttackEnd();

	UFUNCTION()
	void AnimNotify_Death();

	UFUNCTION()
	void AnimNotify_HitEnd();
};
