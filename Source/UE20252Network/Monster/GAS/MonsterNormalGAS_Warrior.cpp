// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterNormalGAS_Warrior.h"
#include "AIController.h"

AMonsterNormalGAS_Warrior::AMonsterNormalGAS_Warrior()
{
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> MeshAsset(TEXT(
		"/Script/Engine.SkeletalMesh'/Game/ParagonMinions/Characters/Minions/Down_Minions/Meshes/Minion_Lane_Melee_Dawn.Minion_Lane_Melee_Dawn'"));

	if (MeshAsset.Succeeded())
	{
		mMesh->SetSkeletalMeshAsset(MeshAsset.Object);
	}

	mBody->SetCapsuleHalfHeight(80.f);
	mBody->SetCapsuleRadius(45.f);

	mMesh->SetRelativeLocation(FVector(0.0, 0.0, -80.0));
	mMesh->SetRelativeRotation(FRotator(0.0, -90.0, 0.0));

	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimClass(
		TEXT("/Script/Engine.AnimBlueprint'/Game/Monster/ABP_MinionWarrior.ABP_MinionWarrior_C'"));

	if (AnimClass.Succeeded())
	{
		mMesh->SetAnimInstanceClass(AnimClass.Class);
	}

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

	auto* AIController = GetController<AAIController>();
	if (IsValid(AIController))
	{
		auto* Target = Cast<AActor>(AIController->GetBlackboardComponent()->GetValueAsObject(TEXT("AttackTarget")));
		if (IsValid(Target))
		{
			// 공격 어빌리티를 발동시킨다.
			FGameplayEventData EventData;
			EventData.Target = Target;
			EventData.Instigator = this;

			EventData.EventTag = FGameplayTag::RequestGameplayTag(TEXT("Ability.Monster.Attack"));

			FHitResult HitResult;
			HitResult.bBlockingHit = true;
			HitResult.Location = Target->GetActorLocation();
			HitResult.ImpactPoint = Target->GetActorLocation();
			HitResult.TraceStart = GetActorLocation();
			HitResult.TraceEnd = HitResult.Location;
			HitResult.Component = Cast<UPrimitiveComponent>(Target->GetRootComponent());
			HitResult.HitObjectHandle = FActorInstanceHandle(Target);

			auto* TargetData = new FGameplayAbilityTargetData_SingleTargetHit(HitResult);

			EventData.TargetData.Add(TargetData);

			UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(this, EventData.EventTag, EventData);

			GEngine->AddOnScreenDebugMessage(-1, 1000.f, FColor::Red, TEXT("Monster NormalAttack"));
		}
	}
}
