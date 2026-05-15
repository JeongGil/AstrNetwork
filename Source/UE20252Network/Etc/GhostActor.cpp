// Fill out your copyright notice in the Description page of Project Settings.


#include "GhostActor.h"

// Sets default values
AGhostActor::AGhostActor()
{
	PrimaryActorTick.bCanEverTick = true;

	mRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	mMesh = CreateDefaultSubobject<UPoseableMeshComponent>(TEXT("Mesh"));

	SetRootComponent(mRoot);

	mMesh->SetupAttachment(mRoot);

	mMesh->SetRelativeRotation(FRotator(0.0, -90.0, 0.0));

	static ConstructorHelpers::FObjectFinder<UMaterialInterface>	SourceMtrl(TEXT("/Script/Engine.Material'/Game/Materials/MT_Ghost.MT_Ghost'"));

	if (SourceMtrl.Succeeded())
		mSourceMaterial = SourceMtrl.Object;
}

// Called when the game starts or when spawned
void AGhostActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGhostActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	mOpacity -= DeltaTime / 3.f;

	if (mOpacity <= 0.f)
	{
		Destroy();
		mOpacity = 0.f;
	}

	for (auto& Mtrl : mMaterials)
	{
		Mtrl->SetScalarParameterValue(TEXT("Opacity"), mOpacity);
	}
}

void AGhostActor::CopyMesh(USkeletalMeshComponent* Mesh)
{
	// SkeletalMesh를 지정한다.
	mMesh->SetSkeletalMesh(Mesh->SkeletalMesh);

	// Pose를 복사한다.
	mMesh->CopyPoseFromSkeletalComponent(Mesh);

	int32	MaterialCount = mMesh->GetNumMaterials();

	for (int32 i = 0; i < MaterialCount; ++i)
	{
		UMaterialInstanceDynamic* Mtrl = mMesh->CreateDynamicMaterialInstance(i,
			mSourceMaterial);

		Mtrl->BlendMode = EBlendMode::BLEND_TranslucentGreyTransmittance;

		mMaterials.Add(Mtrl);
	}
}
