// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PlayerCharacterGAS.h"
#include "WraithGAS.generated.h"

/**
 *
 */
UCLASS()
class UE20252NETWORK_API AWraithGAS : public APlayerCharacterGAS
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AWraithGAS();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void InputAttack();

	UFUNCTION(Server, Reliable)
	void Attack_Server();
	void Attack_Server_Implementation();

	UFUNCTION(NetMulticast, Reliable)
	void Attack_Execution();
	void Attack_Execution_Implementation();

public:
	virtual void NormalAttack();

	virtual void SvrNormalAttack_Implementation() override;
};
