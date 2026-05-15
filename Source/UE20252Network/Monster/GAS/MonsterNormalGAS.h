// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MonsterGAS.h"
#include "MonsterNormalGAS.generated.h"

/**
 * 
 */
UCLASS()
class UE20252NETWORK_API AMonsterNormalGAS : public AMonsterGAS
{
	GENERATED_BODY()

public:
	AMonsterNormalGAS();

protected:
	virtual void BeginPlay() override;

public:
	virtual void OnConstruction(const FTransform& Transform);
	virtual void Tick(float DeltaTime) override;
	virtual void PossessedBy(AController* NewController);
	virtual void UnPossessed();

public:
	virtual void NormalAttack();
};
