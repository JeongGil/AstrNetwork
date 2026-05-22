// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectileBase.h"
#include "WraithBullet.generated.h"

UCLASS()
class UE20252NETWORK_API AWraithBullet : public AProjectileBase
{
	GENERATED_BODY()

public:
	AWraithBullet();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UParticleSystemComponent> mParticle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UParticleSystem> mHitParticle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USoundBase> mHitSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UMaterialInterface> mHitDecal;

	float mAttack = 0.f;
	AController* mOwnerController = nullptr;

public:
	void SetAttack(float Attack)
	{
		mAttack = Attack;
	}

	void SetOwnerController(AController* Controller)
	{
		mOwnerController = Controller;
	}

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

public:
	UFUNCTION()
	void BulletHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	               FVector NormalImpulse, const FHitResult& Hit);

protected:
	UFUNCTION(NetMulticast, Unreliable)
	void Multi_PlayHitEffects(const FVector& Location, const FRotator& Rotation);
};
