// Fill out your copyright notice in the Description page of Project Settings.


#include "DecalBase.h"

// Sets default values
ADecalBase::ADecalBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	mDecal = CreateDefaultSubobject<UDecalComponent>(TEXT("Decal"));

	SetRootComponent(mDecal);

	//SetActorRotation(FRotator(-90.0, 0.0, 0.0));
}

// Called when the game starts or when spawned
void ADecalBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADecalBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADecalBase::SetDecalMaterial(UMaterialInterface* Material)
{
	mDecal->SetDecalMaterial(Material);
}

void ADecalBase::SetDecalMaterial(const FString& MaterialPath)
{
	TObjectPtr<UMaterialInterface>	Material = LoadObject<UMaterialInterface>(
		GetWorld(), MaterialPath);

	mDecal->SetDecalMaterial(Material);
}
