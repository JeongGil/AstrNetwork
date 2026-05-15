// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterNormalGAS.h"
#include "MonsterGASController.h"

AMonsterNormalGAS::AMonsterNormalGAS()
{
}

void AMonsterNormalGAS::BeginPlay()
{
	Super::BeginPlay();
}

void AMonsterNormalGAS::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
}

void AMonsterNormalGAS::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


void AMonsterNormalGAS::PossessedBy(AController* NewController)
{
	//UE_LOG(UELOG, Warning, TEXT("Monster PossessedBy"));
	GEngine->AddOnScreenDebugMessage(-1, 1000.f, FColor::Green,
		TEXT("Monster PossessedBy"));

	AMonsterGASController* Ctrl = Cast<AMonsterGASController>(NewController);

	Ctrl->SetAITree(TEXT("/Script/AIModule.BehaviorTree'/Game/Monster/BT_MonsterGAS_Normal.BT_MonsterGAS_Normal'"));

	Super::PossessedBy(NewController);
}

void AMonsterNormalGAS::UnPossessed()
{
	Super::UnPossessed();
}

void AMonsterNormalGAS::NormalAttack()
{
	Super::NormalAttack();

	/*TObjectPtr<AAIController>	AIController = GetController<AAIController>();

	if (IsValid(AIController))
	{
		AActor* Target = Cast<AActor>(AIController->GetBlackboardComponent()->GetValueAsObject(TEXT("AttackTarget")));

		if (Target)
		{
			FDamageEvent	DmgEvent;
			Target->TakeDamage(mAttack, DmgEvent, GetController(), this);
		}
	}*/
}


