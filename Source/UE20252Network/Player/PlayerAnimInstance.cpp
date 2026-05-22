// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerAnimInstance.h"
#include "GAS/PlayerCharacterGAS.h"

void UPlayerAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	// 델리게이트. 언리얼엔진에서 함수 포인터를 등록하고 호출해주기 위해서 제공되는 객체이다.
	OnMontageEnded.AddDynamic(this, &UPlayerAnimInstance::MontageEnd);
}

void UPlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	TObjectPtr<APlayerCharacterGAS>	PlayerChar = Cast<APlayerCharacterGAS>(TryGetPawnOwner());

	if (IsValid(PlayerChar))
	{
		//UE_LOG(UELOG, Warning, TEXT("Anim"));
		// Movement를 얻어온다.
		UCharacterMovementComponent* Movement = PlayerChar->GetCharacterMovement();

		// 이동속도
		mMoveSpeed = Movement->Velocity.Length();

		// 공중에 떠있는지
		mIsInAir = Movement->IsFalling();

		// 가속도가 있는지
		float Acceleration = Movement->GetCurrentAcceleration().Length();

		mAccelerating = Acceleration > 0.f;

		FRotator	CurrentRot = PlayerChar->GetActorRotation();

		FRotator	DeltaRot = UKismetMathLibrary::NormalizedDeltaRotator(CurrentRot, mPrevRotator);

		float DeltaYaw = DeltaRot.Yaw / DeltaSeconds / 7.f;

		mYawDelta = FMath::FInterpTo(mYawDelta, DeltaYaw, DeltaSeconds, 6.f);


		// 회전 정보를 저장한다.
		mPrevRotator = CurrentRot;
	}
}

void UPlayerAnimInstance::PlayAttack()
{
	// 공격 몽타주가 있을 경우 공격 몽타주를 재생한다.
	if (!IsValid(mAttackMontage))
	{
		UE_LOG(UELOG, Warning, TEXT("Not Valid Attack Montage"));

		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Not Valid Attack Montage"));
		}

		return;
	}

	if (mAttackEnable)
	{
		// 이 몽타주가 재생되고 있는지를 판단한다.
		if (!Montage_IsPlaying(mAttackMontage))
		{
			mAttackEnable = false;

			// 시작 위치를 0으로 만들어준다.
			Montage_SetPosition(mAttackMontage, 0.f);

			Montage_Play(mAttackMontage, 1.f);

			// 섹션을 원하는 섹션으로 이동할 수 있다.
			if (mAttackSection.Num() > 0)
			{
				Montage_JumpToSection(mAttackSection[0], mAttackMontage);
			}
		}
	}

	// 콤보가 가능한 상태라면
	else if (mComboEnable)
	{
		if (mAttackSection.Num() > 0)
		{
			// Num() : 배열 수를 반환한다.
			mAttackIndex = (mAttackIndex + 1) % mAttackSection.Num();

			Montage_Play(mAttackMontage, 1.f);

			// 섹션을 원하는 섹션으로 이동할 수 있다.
			Montage_JumpToSection(mAttackSection[mAttackIndex], mAttackMontage);
		}

		mComboEnable = false;
	}
}

// 정점 * 월드 * 뷰 * 투영 = 투영정점
// 투영정점 / w = 원근감이 적용된 정점
// -1 ~ 1, 1280 x 720일 경우
// x * 640 + 640 = sx
// y * -360 + 360 = sy
// 마우스 위치 mx, my
// (mx / 640 - 1) * w
// (my / -360 + 1) * w
void UPlayerAnimInstance::PlaySkill1()
{
	if (!IsValid(mSkill1Montage))
	{
		return;
	}

	// 시작 위치를 0으로 만들어준다.
	Montage_SetPosition(mSkill1Montage, 0.f);

	Montage_Play(mSkill1Montage, 1.f);

	// 섹션을 원하는 섹션으로 이동할 수 있다.
	if (mSkill1Section.Num() > 0)
	{
		Montage_JumpToSection(mSkill1Section[mSkill1Index], mSkill1Montage);
	}
}

void UPlayerAnimInstance::ClearSkill1()
{
	mSkill1Index = 0;
}

void UPlayerAnimInstance::AnimNotify_ComboStart()
{
	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, TEXT("Combo Start Notify"));

	mComboEnable = true;
}

void UPlayerAnimInstance::AnimNotify_ComboEnd()
{
	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, TEXT("Combo End Notify"));

	mComboEnable = false;
}

void UPlayerAnimInstance::MontageEnd(UAnimMontage* Montage, bool bInterrupted)
{
	MontageEndOverride(Montage, bInterrupted);
}

void UPlayerAnimInstance::MontageEndOverride(UAnimMontage* Montage, bool bInterrupted)
{
	if (Montage == mAttackMontage)
	{
		if (!bInterrupted)
		{
			mAttackEnable = true;
			mComboEnable = false;
			mAttackIndex = 0;
		}
	}

	else if (Montage == mSkill1Montage)
	{
		if (!bInterrupted)
		{
			mAttackEnable = true;
			mComboEnable = false;
			mAttackIndex = 0;
			// Casting 모션에서 Loop 모션으로 전환.
			//if (mSkill1Index == 0 ||
			//	mSkill1Index == 1)
			//{
			//	mSkill1Index = 1;

			//	// 시작 위치를 0으로 만들어준다.
			//	Montage_SetPosition(mSkill1Montage, 0.f);

			//	Montage_Play(mSkill1Montage, 1.f);

			//	// 섹션을 원하는 섹션으로 이동할 수 있다.
			//	Montage_JumpToSection(mSkill1Section[mSkill1Index], mSkill1Montage);
			//}
		}

		//else
		//	mSkill1Index = 0;
	}
}
