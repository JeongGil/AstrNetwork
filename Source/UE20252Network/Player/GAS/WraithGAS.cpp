// Fill out your copyright notice in the Description page of Project Settings.


#include "WraithGAS.h"
#include "../PlayerAnimInstance.h"
#include "../MainPlayerState.h"

AWraithGAS::AWraithGAS()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// SkeletalMesh를 지정한다.
	// Mesh의 참조를 얻어온다.
	static ConstructorHelpers::FObjectFinder<USkeletalMesh>	MeshAsset(TEXT("/Script/Engine.SkeletalMesh'/Game/ParagonWraith/Characters/Heroes/Wraith/Skins/LunarOps/Meshes/Wraith_LunarOps.Wraith_LunarOps'"));

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
	static ConstructorHelpers::FClassFinder<UAnimInstance>	AnimClass(TEXT("/Script/Engine.AnimBlueprint'/Game/Player/Wraith/ABPWraithTemplate.ABPWraithTemplate_C'"));

	// SkeletalMeshComponent에 애니메이션 블루프린트 클래스를 지정하여 사용하게 한다.
	if (AnimClass.Succeeded())
		GetMesh()->SetAnimInstanceClass(AnimClass.Class);

	mHeadUpWidget->SetRelativeLocation(FVector(0.0, 0.0, 190.0));

	mPlayerName = TEXT("Wraith");
}

// Called when the game starts or when spawned
void AWraithGAS::BeginPlay()
{
	// Super : 부모클래스를 의미한다.
	Super::BeginPlay();

}

// Called every frame
void AWraithGAS::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWraithGAS::InputAttack()
{
	mAnimInst->PlayAttack();
}

void AWraithGAS::NormalAttack()
{
	// 총알을 스폰시킨다.
	/*FVector	MuzzleLoc = GetMesh()->GetSocketLocation(TEXT("Muzzle_01"));

	FActorSpawnParameters	param;

	param.SpawnCollisionHandlingOverride =
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	TObjectPtr<AWraithBullet>	Bullet = GetWorld()->SpawnActor<AWraithBullet>(
		MuzzleLoc, GetActorRotation(), param);

	Bullet->SetAttack(GetPlayerState<AMainPlayerState>()->GetAttack());
	Bullet->SetOwnerController(GetController());*/
}


