// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerTemplateAnimInstance.h"
#include "GAS/PlayerCharacterGAS.h"

void UPlayerTemplateAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();
}

void UPlayerTemplateAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
}

void UPlayerTemplateAnimInstance::AnimNotify_SkillCasting()
{
	TObjectPtr<APlayerCharacterGAS>	PlayerChar = Cast<APlayerCharacterGAS>(TryGetPawnOwner());

	if (IsValid(PlayerChar))
	{
		PlayerChar->Skill1Casting();
	}

	mSkill1Index = (mSkill1Index + 1) % mSkill1Section.Num();
}

void UPlayerTemplateAnimInstance::MontageEndOverride(UAnimMontage* Montage,
	bool bInterrupted)
{
	Super::MontageEndOverride(Montage, bInterrupted);

	/*TObjectPtr<APlayerCharacterGAS>	PlayerChar = Cast<APlayerCharacterGAS>(TryGetPawnOwner());

	if (Montage == mSkill1Montage)
	{
		if (mSkill1Index == 0)
		{
			if (IsValid(PlayerChar))
			{
				PlayerChar->Skill1Casting();
			}

			mSkill1Index = (mSkill1Index + 1) % mSkill1Section.Num();
		}
	}*/
}
