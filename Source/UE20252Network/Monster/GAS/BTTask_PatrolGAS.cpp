// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_PatrolGAS.h"
#include "MonsterGAS.h"
#include "AIController.h"

UBTTask_PatrolGAS::UBTTask_PatrolGAS()
{
	NodeName = TEXT("MonsterPatrolGAS");
	// Tick 호출
	bNotifyTick = true;
	// TaskFinished 호출
	bNotifyTaskFinished = true;
}

EBTNodeResult::Type UBTTask_PatrolGAS::ExecuteTask(UBehaviorTreeComponent& OwnerComp,
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

	if (Target)
		return EBTNodeResult::Succeeded;

	// 몬스터 객체를 얻어온다.
	AMonsterGAS* Monster = AIController->GetPawn<AMonsterGAS>();

	if (!Monster)
		return EBTNodeResult::Failed;

	// 몬스터가 순찰을 해야 되는 몬스터인지 판단한다.
	if (!Monster->GetPatrolEnable())
		return EBTNodeResult::Failed;

	// AIController는 NavAgent를 다중상속받아 구현되어 있다.
	EPathFollowingRequestResult::Type MoveResult = AIController->MoveToLocation(Monster->GetPatrolPoint());

	if (MoveResult == EPathFollowingRequestResult::Failed)
		return EBTNodeResult::Failed;

	Monster->ChangeAnim(EMonsterNormalAnim::Walk);

	return EBTNodeResult::InProgress;
}

void UBTTask_PatrolGAS::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory,
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

	if (Target)
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

	EPathFollowingStatus::Type PathStatus = AIController->GetMoveStatus();

	if (PathStatus == EPathFollowingStatus::Idle)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	}

	// 공격거리 안에 들어갔는지 판단한다.
	// 두 객체의 캡슐을 얻어와서 높이를 맞추고 거리를 구한다.
	FVector	TargetLocation, MonsterLocation;

	TargetLocation = Monster->GetPatrolPoint();

	MonsterLocation = Monster->GetActorLocation();

	UCapsuleComponent* Capsule = Cast<UCapsuleComponent>(Monster->GetRootComponent());

	if (Capsule)
		MonsterLocation.Z -= Capsule->GetScaledCapsuleHalfHeight();

	// 두 지점 사이의 거리를 구한다.
	float	Distance = FVector::Dist(MonsterLocation, TargetLocation);

	if (Distance <= 5.f)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	}
}

void UBTTask_PatrolGAS::OnTaskFinished(UBehaviorTreeComponent& OwnerComp,
	uint8* NodeMemory, EBTNodeResult::Type TaskResult)
{
	// 이동을 멈춘다.
	AAIController* AIController = OwnerComp.GetAIOwner();

	AIController->StopMovement();

	AMonsterGAS* Monster = AIController->GetPawn<AMonsterGAS>();

	if (Monster)
	{
		if (Monster->GetPatrolEnable())
		{
			Monster->NextPatrol();
		}
	}
}
