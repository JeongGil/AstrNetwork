// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PlayerCharacterGAS.h"
#include "ShinbiGAS.generated.h"

/**
 * 
 */
UCLASS()
class UE20252NETWORK_API AShinbiGAS : public APlayerCharacterGAS
{
	GENERATED_BODY()

public:
	AShinbiGAS();

protected:
	TObjectPtr<AActor>	mMagicCircleActor;

	bool		mEnableGhost = false;
	int32		mGhostCount = 0;
	FTimerHandle	mGhostTimer;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void InputAttack();
	virtual void Skill1();
	virtual void Skill3();

public:
	virtual void NormalAttack();
	virtual void Skill1Casting();

protected:
	void GhostSpawn();

protected:
	UFUNCTION(Server, Reliable)
	void Attack_Server();
	void Attack_Server_Implementation();

	UFUNCTION(NetMulticast, Reliable)
	void Attack_Execution();
	void Attack_Execution_Implementation();
};
