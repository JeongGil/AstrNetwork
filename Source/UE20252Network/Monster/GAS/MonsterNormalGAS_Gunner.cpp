// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterNormalGAS_Gunner.h"
#include "AIController.h"

AMonsterNormalGAS_Gunner::AMonsterNormalGAS_Gunner()
{
	static ConstructorHelpers::FObjectFinder<USkeletalMesh>	MeshAsset(TEXT("/Script/Engine.SkeletalMesh'/Game/ParagonMinions/Characters/Minions/Down_Minions/Meshes/Minion_Lane_Ranged_Dawn.Minion_Lane_Ranged_Dawn'"));

	if (MeshAsset.Succeeded())
		mMesh->SetSkeletalMeshAsset(MeshAsset.Object);

	mBody->SetCapsuleHalfHeight(80.f);
	mBody->SetCapsuleRadius(45.f);

	mMesh->SetRelativeLocation(FVector(0.0, 0.0, -80.0));
	mMesh->SetRelativeRotation(FRotator(0.0, -90.0, 0.0));

	static ConstructorHelpers::FClassFinder<UAnimInstance>	AnimClass(TEXT("/Script/Engine.AnimBlueprint'/Game/Monster/ABP_MinionGunner.ABP_MinionGunner_C'"));

	if (AnimClass.Succeeded())
		mMesh->SetAnimInstanceClass(AnimClass.Class);

	mDataName = TEXT("MinionGunner");
}

void AMonsterNormalGAS_Gunner::BeginPlay()
{
	Super::BeginPlay();
}

void AMonsterNormalGAS_Gunner::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
}

void AMonsterNormalGAS_Gunner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMonsterNormalGAS_Gunner::NormalAttack()
{
	Super::NormalAttack();

	TObjectPtr<AAIController>	AIController = GetController<AAIController>();

	if (IsValid(AIController))
	{
		// 이펙트 처리
		UParticleSystem* Particle = LoadObject<UParticleSystem>(GetWorld(), TEXT("/Script/Engine.ParticleSystem'/Game/ParagonMinions/FX/Particles/Minions/Minion_melee/FX/Impacts/P_Minion_Impact_Default.P_Minion_Impact_Default'"));

		AActor* Target = Cast<AActor>(AIController->GetBlackboardComponent()->GetValueAsObject(TEXT("AttackTarget")));

		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Particle,
			Target->GetActorLocation());
	}
}
