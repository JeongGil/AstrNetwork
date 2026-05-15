// Fill out your copyright notice in the Description page of Project Settings.


#include "ShinbiGAS.h"
#include "../../Etc/DecalBase.h"
#include "../../Etc/GeometryActor.h"
#include "../PlayerAnimInstance.h"
#include "../MainPlayerState.h"
#include "Ability/GameplayAbility_ShinbiSkill2.h"
#include "../../Etc/GhostActor.h"

// Sets default values
AShinbiGAS::AShinbiGAS()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// SkeletalMesh를 지정한다.
	// Mesh의 참조를 얻어온다.
	static ConstructorHelpers::FObjectFinder<USkeletalMesh>	MeshAsset(TEXT("/Script/Engine.SkeletalMesh'/Game/ParagonShinbi/Characters/Heroes/Shinbi/Skins/Tier_1/Shinbi_Dynasty/Meshes/ShinbiDynasty.ShinbiDynasty'"));

	// GetMesh() : ACharacter 클래스에서 제공하는 SkeletalMeshComponent를 얻어오기 위한 함수이다.
	// SkeletalMeshComponent는 ACharacter 클래스에서 private으로 되어 있기 때문에 GetMesh를
	// 이용해서 사용해야 한다.
	// FObjectFinder의 Succeeded 함수로 로딩이 성공했는지를 판단한고 FObjectFinder의 Object 변수를
	// 지정한다. 실제 에셋은 Object 변수에 저장된다.
	if (MeshAsset.Succeeded())
		GetMesh()->SetSkeletalMeshAsset(MeshAsset.Object);

	// CapsuleComponent의 높이와 둘레를 지정한다.
	GetCapsuleComponent()->SetCapsuleHalfHeight(95.f);
	GetCapsuleComponent()->SetCapsuleRadius(28.f);

	GetMesh()->SetRelativeLocation(FVector(0.0, 0.0, -95.0));
	GetMesh()->SetRelativeRotation(FRotator(0.0, -90.0, 0.0));

	// 애니메이션 블루프린트 클래스를 얻어온다.
	// UClass를 얻어올때는 마지막에 반드시 _C 를 붙여주어야 한다.
	/*static ConstructorHelpers::FClassFinder<UAnimInstance>	AnimClass(TEXT("/Script/Engine.AnimBlueprint'/Game/Player/Shinbi/ABPShinbiTest.ABPShinbiTest_C'"));*/
	static ConstructorHelpers::FClassFinder<UAnimInstance>	AnimClass(TEXT("/Script/Engine.AnimBlueprint'/Game/Player/Shinbi/ABPShinbiTemplate1.ABPShinbiTemplate1_C'"));

	// SkeletalMeshComponent에 애니메이션 블루프린트 클래스를 지정하여 사용하게 한다.
	if (AnimClass.Succeeded())
		GetMesh()->SetAnimInstanceClass(AnimClass.Class);

	mHeadUpWidget->SetRelativeLocation(FVector(0.0, 0.0, 190.0));

	mPlayerName = TEXT("Shinbi");
}

// Called when the game starts or when spawned
void AShinbiGAS::BeginPlay()
{
	// Super : 부모클래스를 의미한다.
	Super::BeginPlay();

	mASC->GiveAbility(FGameplayAbilitySpec(UGameplayAbility_ShinbiSkill2::StaticClass(),
		1, (int32)EAbilityInputID::Skill2, this));
}

// Called every frame
void AShinbiGAS::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//if (IsValid(mMagicCircleActor))
	//{
	//	TObjectPtr<APlayerController>	PlayerCtrl = GetController<APlayerController>();

	//	// Picking
	//	FHitResult	Hit;
	//	bool Pick = PlayerCtrl->GetHitResultUnderCursor(
	//		ECollisionChannel::ECC_GameTraceChannel5, true, Hit);

	//	if (Pick)
	//	{
	//		mMagicCircleActor->SetActorLocation(Hit.ImpactPoint);
	//	}
	//}
}

void AShinbiGAS::InputAttack()
{
	Attack_Server();
	//if (IsValid(mMagicCircleActor))
	//{
	//	mAnimInst->PlaySkill1();

	//	mAnimInst->ClearSkill1();

	//	// 스킬 액터 생성
	//	FVector	Loc = mMagicCircleActor->GetActorLocation() + FVector(0.0, 0.0, 1000.0);

	//	FActorSpawnParameters	param;

	//	param.SpawnCollisionHandlingOverride =
	//		ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	//	TObjectPtr<AGeometryActor>	SkillActor = GetWorld()->SpawnActor<AGeometryActor>(Loc, FRotator::ZeroRotator, param);

	//	SkillActor->SetGeometryAsset(TEXT("/Script/GeometryCollectionEngine.GeometryCollection'/Game/Blueprints/GC_SM_PROP_barrel_dungeon_01.GC_SM_PROP_barrel_dungeon_01'"));

	//	mMagicCircleActor->Destroy();
	//}

	//else
	//{
	//	mAnimInst->PlayAttack();
	//}
}

void AShinbiGAS::Skill1()
{
	if (!IsValid(mMagicCircleActor))
	{
		mAnimInst->PlaySkill1();
	}
}

void AShinbiGAS::Skill3()
{
	if (!mEnableGhost)
	{
		mEnableGhost = true;

		GetWorldTimerManager().SetTimer(mGhostTimer, this, &AShinbiGAS::GhostSpawn,
			0.1f, true);
	}
}

void AShinbiGAS::NormalAttack()
{
	TArray<FHitResult>	HitArray;

	FVector	StartLoc = GetActorLocation() + GetActorForwardVector() * 100.f;
	FVector	EndLoc = StartLoc + GetActorForwardVector() * 200.f;

	FQuat	CapsuleRot = FQuat(FRotator(90.0, 0.0, 0.0));

	//CapsuleRot = FRotationMatrix::MakeFromX(GetActorForwardVector()).ToQuat();
	//CapsuleRot = GetActorForwardVector().Rotation().Quaternion();
	CapsuleRot = FQuat::FindBetweenNormals(FVector::UpVector,
		GetActorForwardVector());

	GEngine->AddOnScreenDebugMessage(-1, 100.f, FColor::Green, CapsuleRot.Rotator().ToString());

	// 캡슐을 만들어서 이 모양으로 충돌.
	FCollisionQueryParams	param(NAME_None, false, this);

	// 충돌에서 제외할 액터를 등록할 수 있다.
	//param.AddIgnoredActor(this);

	bool Collision = GetWorld()->SweepMultiByChannel(HitArray, StartLoc, EndLoc,
		CapsuleRot, ECollisionChannel::ECC_GameTraceChannel3,
		FCollisionShape::MakeCapsule(35.f, 100.f), param);

	// 선충돌 할때 사용.
	//GetWorld()->LineTraceMultiByChannel

	FColor	DebugColor = FColor::Green;

	// 충돌 물체가 있을 경우
	if (Collision)
	{
		DebugColor = FColor::Red;

		for (auto Hit : HitArray)
		{
			AActor* HitActor = Hit.GetActor();

			if (!HitActor)
				continue;

			// 공격 어빌리티를 발동시킨다.
			FGameplayEventData	EventData;

			EventData.Target = HitActor;
			EventData.Instigator = this;

			EventData.EventTag = FGameplayTag::RequestGameplayTag(TEXT("Ability.Attack"));

			FGameplayAbilityTargetData_SingleTargetHit* TargetData =
				new FGameplayAbilityTargetData_SingleTargetHit(Hit);

			EventData.TargetData.Add(TargetData);

			UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(this,
				EventData.EventTag, EventData);

			//mASC->TryActivateAbilitiesByTag()
			//mASC->TryActivateAbilityByClass()
			//mASC->TryActivateAbility()

			/*float	Attack = GetPlayerState<AMainPlayerState>()->GetAttack();

			FDamageEvent	DmgEvent;
			Hit.GetActor()->TakeDamage(Attack, DmgEvent, GetController(), this);

			TObjectPtr<USoundBase> Sound = LoadObject<USoundBase>(GetWorld(), TEXT("/Script/Engine.SoundWave'/Game/Sound/Effect/Fire1.Fire1'"));

			if (IsValid(Sound))
			{
				UGameplayStatics::SpawnSoundAtLocation(GetWorld(), Sound,
					Hit.ImpactPoint);
			}

			TObjectPtr<UParticleSystem>	Particle = LoadObject<UParticleSystem>(
				GetWorld(), TEXT("/Script/Engine.ParticleSystem'/Game/ParagonShinbi/FX/Particles/Abilities/Primary/FX/P_Mudang_Primary_Impact.P_Mudang_Primary_Impact'"));

			if (IsValid(Particle))
			{
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Particle,
					Hit.ImpactPoint);
			}*/
		}
	}

#if WITH_EDITOR

	DrawDebugCapsule(GetWorld(), (StartLoc + EndLoc) * 0.5f, 100.f, 35.f, CapsuleRot,
		DebugColor, false, 1.f);

#endif
}

void AShinbiGAS::Skill1Casting()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Casting"));

	TObjectPtr<APlayerController>	PlayerCtrl = GetController<APlayerController>();

	// Picking
	FHitResult	Hit;
	bool Pick = PlayerCtrl->GetHitResultUnderCursor(
		ECollisionChannel::ECC_GameTraceChannel5, true, Hit);

	FVector	DecalLoc;

	if (Pick)
		DecalLoc = Hit.ImpactPoint;

	// 바닥의 마법진을 생성한다.
	FActorSpawnParameters	param;
	param.SpawnCollisionHandlingOverride =
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	TObjectPtr<ADecalBase> DecalActor = GetWorld()->SpawnActor<ADecalBase>(DecalLoc,
		FRotator(-90.0, 0.0, 0.0), param);

	DecalActor->SetDecalMaterial(TEXT("/Script/Engine.Material'/Game/Player/Shinbi/Material/MTShibiMagicCircle.MTShibiMagicCircle'"));

	mMagicCircleActor = DecalActor;

}

void AShinbiGAS::GhostSpawn()
{
	++mGhostCount;

	FVector	Location = GetActorLocation();
	
	Location.Z -= GetCapsuleComponent()->GetScaledCapsuleHalfHeight();

	AGhostActor* Ghost = GetWorld()->SpawnActor<AGhostActor>(Location,
		GetActorRotation());

	Ghost->CopyMesh(GetMesh());

	if (mGhostCount == 30)
	{
		GetWorldTimerManager().ClearTimer(mGhostTimer);
		mEnableGhost = false;
		mGhostCount = 0;
	}
}

void AShinbiGAS::Attack_Server_Implementation()
{
	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, TEXT("Attack Server"));
	Attack_Execution();
}

void AShinbiGAS::Attack_Execution_Implementation()
{
	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, TEXT("Attack Execution"));
	mAnimInst->PlayAttack();
}
