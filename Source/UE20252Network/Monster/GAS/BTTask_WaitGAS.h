// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../GameInfo.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_WaitGAS.generated.h"

UCLASS()
class UE20252NETWORK_API UBTTask_WaitGAS : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_WaitGAS();

public:
	virtual uint16 GetInstanceMemorySize()	const;
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp,
		uint8* NodeMemory);

protected:
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory,
		float DeltaSeconds);
	virtual void OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory,
		EBTNodeResult::Type TaskResult);

protected:
	void WaitFinish(uint8* NodeMemory);
};
