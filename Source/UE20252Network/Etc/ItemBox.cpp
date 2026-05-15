// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemBox.h"
#include "../GameMode/AssetGameInstanceSubsystem.h"
#include "../Player/GAS/PlayerCharacterGAS.h"

// Sets default values
AItemBox::AItemBox()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	mBody = CreateDefaultSubobject<UBoxComponent>(TEXT("Body"));
	mMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));

	SetRootComponent(mBody);

	mMesh->SetupAttachment(mBody);

	static ConstructorHelpers::FObjectFinder<UStaticMesh>	MeshAsset(TEXT("/Script/Engine.StaticMesh'/Game/Fantastic_Dungeon_Pack/meshes/props/container/SM_PROP_box_dungeon_03.SM_PROP_box_dungeon_03'"));

	if (MeshAsset.Succeeded())
		mMesh->SetStaticMesh(MeshAsset.Object);

	mMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	mBody->SetCollisionProfileName(TEXT("ItemBox"));

	mBody->SetBoxExtent(FVector(47.0, 47.0, 42.0));

	mMesh->SetRelativeLocation(FVector(0.0, 0.0, -42.0));

	mBody->OnComponentBeginOverlap.AddDynamic(this, &AItemBox::ItemOverlap);
}

// Called when the game starts or when spawned
void AItemBox::BeginPlay()
{
	Super::BeginPlay();

	StartDropAnimation();
}

// Called every frame
void AItemBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!mDrop)
		return;

	mAnimTime += DeltaTime;

	float Alpha = FMath::Clamp(mAnimTime / mDropDuration, 0.f, 1.f);

	float MoveAlpha = EaseOutCubic(Alpha);

	UE_LOG(UELOG, Warning, TEXT("Alpha : %.5f"), MoveAlpha);

	FVector	DropLocation = FMath::Lerp(mStartLocation, mGroundLocation, MoveAlpha);

	DropLocation.Z += FMath::Sin(Alpha * PI) * mJumpHeight;

	SetActorLocation(DropLocation);

	// 회전.
	FRotator	Rot = GetActorRotation();

	Rot += mSpinSpeed * DeltaTime;
	SetActorRotation(Rot);

	if (Alpha >= 1.f)
	{
		mDrop = false;
		SetActorLocation(mGroundLocation);
		SetActorRotation(FRotator::ZeroRotator);
	}
}

void AItemBox::ItemOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	const FHitResult& SweepResult)
{
	APlayerCharacterGAS* PlayerCharacter = Cast<APlayerCharacterGAS>(OtherActor);

	if (IsValid(PlayerCharacter) && !mDropItemKey.IsNone())
	{
		UGameInstance* GameInst = GetGameInstance();

		UAssetGameInstanceSubsystem* AssetSubSystem =
			GameInst->GetSubsystem<UAssetGameInstanceSubsystem>();

		const FItemTableInfo* Info = AssetSubSystem->FindItemInfo(mDropItemKey);

		if (!Info)
			return;

		PlayerCharacter->Acquisition(Info);
	}

	Destroy();
}

void AItemBox::StartDropAnimation()
{
	// 시작 위치를 저장한다.
	mStartLocation = GetActorLocation();

	// 땅의 위치를 찾는다.
	if (!FindGroundLocation())
	{
		mGroundLocation = mStartLocation + FVector(0.0, 0.0, -100.0);
	}

	mSpinSpeed = FRotator(720.0, 1080.0, 0.0);

	mAnimTime = 0.f;
	mDrop = true;
}

bool AItemBox::FindGroundLocation()
{
	// 현재 위치에서 아래로 충돌처리를 진행한다.
	FVector	TraceStart = GetActorLocation() + FVector(0.0, 0.0, 50.0);
	FVector	TraceEnd = TraceStart - FVector(0.0, 0.0, 500.0);

	FCollisionQueryParams	param(NAME_None, false, this);

	FHitResult	Hit;

	bool Collision = GetWorld()->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd,
		ECollisionChannel::ECC_Visibility, param);

	if (!Collision)
		return false;

	mGroundLocation = Hit.ImpactPoint;

	// 상자의 절반높이만큼 위로 위치를 잡아준다.
	mGroundLocation.Z += mBody->GetScaledBoxExtent().Z;

	return true;
}

float AItemBox::EaseOutCubic(float Alpha)	const
{
	return 1.f - FMath::Pow(1.f - Alpha, mDropPow);
}

void AItemBox::SetItem(const FName& ItemKey)
{
	mDropItemKey = ItemKey;
}
