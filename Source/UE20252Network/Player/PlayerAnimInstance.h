// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PlayerAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class UE20252NETWORK_API UPlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	float	mMoveSpeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float	mViewPitch;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float	mViewYaw;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool	mIsInAir;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool	mAccelerating;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float	mYawDelta;

	FRotator	mPrevRotator = FRotator::ZeroRotator;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAnimMontage>	mAttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TArray<FName>	mAttackSection;

	int32			mAttackIndex = 0;

	bool			mComboEnable = false;

	bool			mAttackEnable = true;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAnimMontage>	mSkill1Montage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TArray<FName>	mSkill1Section;

	int32			mSkill1Index = 0;

public:
	void AddViewPitch(float Pitch)
	{
		mViewPitch += Pitch;
	}

	void AddViewYaw(float Yaw)
	{
		mViewYaw += Yaw;
	}
	
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
	void PlayAttack();
	void PlaySkill1();
	void ClearSkill1();

public:
	// 노티파이 함수 규칙 : AnimNotify_노티파이이름() 으로 함수를 만든다.
	UFUNCTION()
	void AnimNotify_ComboStart();

	UFUNCTION()
	void AnimNotify_ComboEnd();

protected:
	// 델리게이트에 등록하기 위해서는 반드시 UFUNCTION 매크로를 이용한 함수만 등록이 된다.
	UFUNCTION()
	void MontageEnd(UAnimMontage* Montage, bool bInterrupted);

protected:
	virtual void MontageEndOverride(UAnimMontage* Montage, bool bInterrupted);
};
