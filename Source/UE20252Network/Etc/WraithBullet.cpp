// Fill out your copyright notice in the Description page of Project Settings.


#include "WraithBullet.h"


// Sets default values
AWraithBullet::AWraithBullet()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	mParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particle"));

	mParticle->SetupAttachment(mBody);

	static ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleAsset(TEXT(
		"/Script/Engine.ParticleSystem'/Game/ParagonWraith/FX/Particles/Abilities/ScopedShot/FX/P_Wraith_Sniper_Projectile.P_Wraith_Sniper_Projectile'"));

	// SetTemplate 함수는 파티클 에셋을 지정하는 함수이다.
	if (ParticleAsset.Succeeded())
	{
		mParticle->SetTemplate(ParticleAsset.Object);
	}

	mParticle->SetRelativeScale3D(FVector(0.2, 0.5, 0.5));

	mBody->SetBoxExtent(FVector(43.0, 8.0, 10.0));

	SetLifeSpan(5.f);

	mBody->SetCollisionProfileName(TEXT("PlayerAttack"));

	// 충돌체가 Hit 이벤트가 발생할 때 이 함수를 호출해주게 한다.
	mBody->OnComponentHit.AddDynamic(this, &AWraithBullet::BulletHit);

	static ConstructorHelpers::FObjectFinder<UParticleSystem> HitAsset(TEXT(
		"/Script/Engine.ParticleSystem'/Game/ParagonWraith/FX/Particles/Abilities/Primary/FX/P_Wraith_Primary_HitCharacter.P_Wraith_Primary_HitCharacter'"));

	if (HitAsset.Succeeded())
	{
		mHitParticle = HitAsset.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundBase> HitSound(
		TEXT("/Script/Engine.SoundWave'/Game/Sound/Effect/Fire1.Fire1'"));

	if (HitSound.Succeeded())
	{
		mHitSound = HitSound.Object;
	}

	static ConstructorHelpers::FObjectFinder<UMaterialInterface> HitDecal(TEXT(
		"/Script/Engine.MaterialInstanceConstant'/Game/Fab/Megascans/Decals/High_Velocity_Blood_Spatter_sgeoahup/Medium/sgeoahup_tier_2/Materials/MI_sgeoahup.MI_sgeoahup'"));

	if (HitDecal.Succeeded())
	{
		mHitDecal = HitDecal.Object;
	}

	mMovement->ProjectileGravityScale = 0.f;
	mMovement->InitialSpeed = 1000.f;
}

void AWraithBullet::BeginPlay()
{
	Super::BeginPlay();
}

void AWraithBullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AWraithBullet::BulletHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                              FVector NormalImpulse, const FHitResult& Hit)
{
	if (!HasAuthority())
	{
		return;
	}

	if (IsValid(OtherActor))
	{
		FDamageEvent DmgEvent;
		OtherActor->TakeDamage(mAttack, DmgEvent, mOwnerController, this);
	}

	Multi_PlayHitEffects(Hit.ImpactPoint, (-Hit.ImpactNormal).Rotation());

	Destroy();
}

void AWraithBullet::Multi_PlayHitEffects_Implementation(const FVector& Location, const FRotator& Rotation)
{
	// 파티클을 재생시킨다.
	if (IsValid(mHitParticle))
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), mHitParticle, Location);
	}

	// 사운드를 재생시킨다.
	if (IsValid(mHitSound))
	{
		UGameplayStatics::SpawnSoundAtLocation(GetWorld(), mHitSound, Location);
	}

	if (IsValid(mHitDecal))
	{
		UGameplayStatics::SpawnDecalAtLocation(GetWorld(), mHitDecal, FVector(20.0, 20.0, 10.0), Location,
		                                       Rotation, 5.f);
	}
}
