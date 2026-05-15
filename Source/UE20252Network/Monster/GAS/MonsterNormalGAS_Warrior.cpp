// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterNormalGAS_Warrior.h"
#include "AIController.h"

AMonsterNormalGAS_Warrior::AMonsterNormalGAS_Warrior()
{
	static ConstructorHelpers::FObjectFinder<USkeletalMesh>	MeshAsset(TEXT("/Script/Engine.SkeletalMesh'/Game/ParagonMinions/Characters/Minions/Down_Minions/Meshes/Minion_Lane_Melee_Dawn.Minion_Lane_Melee_Dawn'"));

	if (MeshAsset.Succeeded())
		mMesh->SetSkeletalMeshAsset(MeshAsset.Object);

	mBody->SetCapsuleHalfHeight(80.f);
	mBody->SetCapsuleRadius(45.f);

	mMesh->SetRelativeLocation(FVector(0.0, 0.0, -80.0));
	mMesh->SetRelativeRotation(FRotator(0.0, -90.0, 0.0));

	static ConstructorHelpers::FClassFinder<UAnimInstance>	AnimClass(TEXT("/Script/Engine.AnimBlueprint'/Game/Monster/ABP_MinionWarrior.ABP_MinionWarrior_C'"));

	if (AnimClass.Succeeded())
		mMesh->SetAnimInstanceClass(AnimClass.Class);

	mDataName = TEXT("MinionWarrior");
}

void AMonsterNormalGAS_Warrior::BeginPlay()
{
	Super::BeginPlay();
}

void AMonsterNormalGAS_Warrior::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
}

void AMonsterNormalGAS_Warrior::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMonsterNormalGAS_Warrior::NormalAttack()
{
	Super::NormalAttack();

	TObjectPtr<AAIController>	AIController = GetController<AAIController>();

	if (IsValid(AIController))
	{
		AActor* Target = Cast<AActor>(AIController->GetBlackboardComponent()->GetValueAsObject(TEXT("AttackTarget")));

		if (IsValid(Target))
		{
			// 공격 어빌리티를 발동시킨다.
			FGameplayEventData	EventData;

			EventData.Target = Target;
			EventData.Instigator = this;

			EventData.EventTag = FGameplayTag::RequestGameplayTag(TEXT("Ability.Attack"));

			FHitResult	Hit;

			Hit.bBlockingHit = true;
			Hit.Location = Target->GetActorLocation();
			Hit.ImpactPoint = Target->GetActorLocation();
			Hit.TraceStart = GetActorLocation();
			Hit.TraceEnd = Hit.Location;
			Hit.Component = Cast<UPrimitiveComponent>(Target->GetRootComponent());
			Hit.HitObjectHandle = FActorInstanceHandle(Target);

			FGameplayAbilityTargetData_SingleTargetHit* TargetData =
				new FGameplayAbilityTargetData_SingleTargetHit(Hit);

			EventData.TargetData.Add(TargetData);

			UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(this,
				EventData.EventTag, EventData);
			//// 이펙트 처리
			//UParticleSystem* Particle = LoadObject<UParticleSystem>(GetWorld(), TEXT("/Script/Engine.ParticleSystem'/Game/ParagonMinions/FX/Particles/Minions/Minion_melee/FX/Impacts/P_Minion_Impact_Default.P_Minion_Impact_Default'"));

			//UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Particle,
			//	Target->GetActorLocation());
		}
	}
}

