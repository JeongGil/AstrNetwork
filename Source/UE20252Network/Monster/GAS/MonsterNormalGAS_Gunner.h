// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MonsterNormalGAS.h"
#include "MonsterNormalGAS_Gunner.generated.h"

/**
 * 
 */
UCLASS()
class UE20252NETWORK_API AMonsterNormalGAS_Gunner : public AMonsterNormalGAS
{
	GENERATED_BODY()

public:
	AMonsterNormalGAS_Gunner();

protected:
	virtual void BeginPlay() override;

public:
	virtual void OnConstruction(const FTransform& Transform);
	virtual void Tick(float DeltaTime) override;

public:
	virtual void NormalAttack();
};
