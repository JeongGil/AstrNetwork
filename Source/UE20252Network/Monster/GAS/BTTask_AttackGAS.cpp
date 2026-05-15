// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_AttackGAS.h"
#include "MonsterGAS.h"
#include "AIController.h"
#include "MonsterAttributeSet.h"

UBTTask_AttackGAS::UBTTask_AttackGAS()
{
	NodeName = TEXT("MonsterAttackGAS");
	// Tick 호출
	bNotifyTick = true;
	// TaskFinished 호출
	bNotifyTaskFinished = true;
}

EBTNodeResult::Type UBTTask_AttackGAS::ExecuteTask(UBehaviorTreeComponent& OwnerComp,
	uint8* NodeMemory)
{
	// BehaviorTreeComponent를 가지고 있는 AIController를 얻어온다.
	AAIController* AIController = OwnerComp.GetAIOwner();

	if (!AIController)
		return EBTNodeResult::Failed;

	// 블랙보드 컴포넌트를 얻어온다.
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();

	if (!BlackboardComp)
		return EBTNodeResult::Failed;

	// 블랙보드에서 타겟을 얻어온다.
	AActor* Target = Cast<AActor>(BlackboardComp->GetValueAsObject(TEXT("Target")));

	if (!Target)
		return EBTNodeResult::Failed;

	// 몬스터 객체를 얻어온다.
	AMonsterGAS* Monster = AIController->GetPawn<AMonsterGAS>();

	if (!Monster)
		return EBTNodeResult::Failed;

	Monster->ChangeAnim(EMonsterNormalAnim::Attack);

	// 공격 대상을 지정한다.
	BlackboardComp->SetValueAsObject(TEXT("AttackTarget"), Target);

	return EBTNodeResult::InProgress;
}

void UBTTask_AttackGAS::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory,
	float DeltaSeconds)
{
	// BehaviorTreeComponent를 가지고 있는 AIController를 얻어온다.
	AAIController* AIController = OwnerComp.GetAIOwner();

	if (!AIController)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return;
	}

	// 블랙보드 컴포넌트를 얻어온다.
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();

	if (!BlackboardComp)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return;
	}

	// 블랙보드에서 타겟을 얻어온다.
	AActor* Target = Cast<AActor>(BlackboardComp->GetValueAsObject(TEXT("Target")));

	if (!Target)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return;
	}

	// 몬스터 객체를 얻어온다.
	AMonsterGAS* Monster = AIController->GetPawn<AMonsterGAS>();

	if (!Monster)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return;
	}

	// 공격모션이 끝났는지 판단한다.
	bool	AttackEnd = BlackboardComp->GetValueAsBool(TEXT("AttackEnd"));

	if (AttackEnd)
	{
		BlackboardComp->SetValueAsBool(TEXT("AttackEnd"), false);

		// 공격거리 안에 들어갔는지 판단한다.
		// 두 객체의 캡슐을 얻어와서 높이를 맞추고 거리를 구한다.
		FVector	TargetLocation, MonsterLocation;

		TargetLocation = Target->GetActorLocation();

		UCapsuleComponent* Capsule = Cast<UCapsuleComponent>(Target->GetRootComponent());

		if (Capsule)
			TargetLocation.Z -= Capsule->GetScaledCapsuleHalfHeight();

		MonsterLocation = Monster->GetActorLocation();

		Capsule = Cast<UCapsuleComponent>(Monster->GetRootComponent());

		if (Capsule)
			MonsterLocation.Z -= Capsule->GetScaledCapsuleHalfHeight();

		// 두 지점 사이의 거리를 구한다.
		float	Distance = FVector::Dist(MonsterLocation, TargetLocation);

		UMonsterAttributeSet* Attr = Monster->GetAttributeSet();

		if (Distance > Attr->GetAttackDistance())
		{
			FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		}

		// 곻격거리 안에 있다면 대상을 바라보게 회전시킨다.
		else
		{
			FRotator	Rot = GetTargetRotatorYaw(TargetLocation, MonsterLocation);

			Monster->SetActorRotation(Rot);
		}
	}
}

void UBTTask_AttackGAS::OnTaskFinished(UBehaviorTreeComponent& OwnerComp,
	uint8* NodeMemory, EBTNodeResult::Type TaskResult)
{
	// 이동을 멈춘다.
	AAIController* AIController = OwnerComp.GetAIOwner();

	AIController->GetBlackboardComponent()->SetValueAsObject(TEXT("AttackTarget"),
		nullptr);
}
