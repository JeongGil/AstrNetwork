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
	TObjectPtr<AMonsterGAS>	Monster = Cast<AMonsterGAS>(TryGetPawnOwner());

	Monster->NormalAttack();
}

void UMonsterGASAnimInstance::AnimNotify_AttackEnd()
{
	TObjectPtr<AMonsterGAS>	Monster = Cast<AMonsterGAS>(TryGetPawnOwner());

	TObjectPtr<AAIController>	AIController = Monster->GetController<AAIController>();

	AIController->GetBlackboardComponent()->SetValueAsBool(TEXT("AttackEnd"), true);
}

void UMonsterGASAnimInstance::AnimNotify_Death()
{
	TObjectPtr<AMonsterGAS>	Monster = Cast<AMonsterGAS>(TryGetPawnOwner());

	Monster->Death();
}

void UMonsterGASAnimInstance::AnimNotify_HitEnd()
{
	mHitAlpha = 0.f;
}
