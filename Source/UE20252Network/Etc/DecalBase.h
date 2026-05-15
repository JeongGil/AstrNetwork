// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "GameFramework/Actor.h"
#include "DecalBase.generated.h"

UCLASS()
class UE20252NETWORK_API ADecalBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADecalBase();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UDecalComponent> mDecal;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	// BlueprintCallable : 블루프린트에서 호출할 수 있는 함수로 만들어준다.
	UFUNCTION(BlueprintCallable)
	void SetDecalMaterial(UMaterialInterface* Material);

	void SetDecalMaterial(const FString& MaterialPath);
};
