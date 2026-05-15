// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterGASAnimInstance.h"
#include "MonsterGAS.h"
#include "AIController.h"

void UMonsterGASAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();
}

void UMonsterGASAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
}

void UMonsterGASAnimInstance::AnimNotify_Attack()
{
	auto* Monster = Cast<AMonsterGAS>(TryGetPawnOwner());
	if (Monster->HasAuthority())
	{
		Monster->NormalAttack();
	}
}

void UMonsterGASAnimInstance::AnimNotify_AttackEnd()
{
	const auto* Monster = Cast<AMonsterGAS>(TryGetPawnOwner());
	if (Monster->HasAuthority())
	{
		auto* AIController = Monster->GetController<AAIController>();

		AIController->GetBlackboardComponent()->SetValueAsBool(TEXT("AttackEnd"), true);
	}
}

void UMonsterGASAnimInstance::AnimNotify_Death()
{
	auto* Monster = Cast<AMonsterGAS>(TryGetPawnOwner());
	if (Monster->HasAuthority())
	{
		Monster->Death();
	}
}

void UMonsterGASAnimInstance::AnimNotify_HitEnd()
{
	mHitAlpha = 0.f;
}
