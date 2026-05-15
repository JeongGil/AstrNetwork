// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_WaitGAS.h"
#include "MonsterGAS.h"
#include "AIController.h"

UBTTask_WaitGAS::UBTTask_WaitGAS()
{
	NodeName = TEXT("MonsterWaitGAS");
	// Tick 호출
	bNotifyTick = true;
	// TaskFinished 호출
	bNotifyTaskFinished = true;
}

uint16 UBTTask_WaitGAS::GetInstanceMemorySize()	const
{
	return sizeof(FWaitTimer);
}

EBTNodeResult::Type UBTTask_WaitGAS::ExecuteTask(UBehaviorTreeComponent& OwnerComp,
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

	Monster->ChangeAnim(EMonsterNormalAnim::Idle);

	float	WaitTime = BlackboardComp->GetValueAsFloat(TEXT("WaitTime"));

	FWaitTimer* Timer = (FWaitTimer*)NodeMemory;

	Timer->Complete = false;

	OwnerComp.GetWorld()->GetTimerManager().SetTimer(Timer->Timer,
		FTimerDelegate::CreateUObject(this, &UBTTask_WaitGAS::WaitFinish,
			NodeMemory), WaitTime, false);

	return EBTNodeResult::InProgress;
}

void UBTTask_WaitGAS::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory,
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

	FWaitTimer* Timer = (FWaitTimer*)NodeMemory;

	if (Timer->Complete)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	}
}

void UBTTask_WaitGAS::OnTaskFinished(UBehaviorTreeComponent& OwnerComp,
	uint8* NodeMemory, EBTNodeResult::Type TaskResult)
{
	FWaitTimer* Timer = (FWaitTimer*)NodeMemory;

	if (Timer->Timer.IsValid())
	{
		OwnerComp.GetWorld()->GetTimerManager().ClearTimer(Timer->Timer);
	}
}

void UBTTask_WaitGAS::WaitFinish(uint8* NodeMemory)
{
	FWaitTimer* Timer = (FWaitTimer*)NodeMemory;

	Timer->Complete = true;
}
