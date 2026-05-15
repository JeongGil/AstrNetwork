// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "Components/PoseableMeshComponent.h"
#include "GameFramework/Actor.h"
#include "GhostActor.generated.h"

UCLASS()
class UE20252NETWORK_API AGhostActor : public AActor
{
	GENERATED_BODY()
	
public:
	AGhostActor();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USceneComponent> mRoot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UPoseableMeshComponent> mMesh;

	TArray<TObjectPtr<UMaterialInstanceDynamic>>	mMaterials;

	TObjectPtr<UMaterialInterface>	mSourceMaterial;

	float		mOpacity = 1.f;

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

public:
	void CopyMesh(USkeletalMeshComponent* Mesh);
};
