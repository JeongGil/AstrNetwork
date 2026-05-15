// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterSpawnPoint.h"
#include "GAS/MonsterGAS.h"

// Sets default values
AMonsterSpawnPoint::AMonsterSpawnPoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	mRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	mPatrolPath = CreateDefaultSubobject<USplineComponent>(TEXT("PatrolPath"));

	SetRootComponent(mRoot);

	mPatrolPath->SetupAttachment(mRoot);

#if WITH_EDITORONLY_DATA

	// 에디터에서 어디 있는지 표시해준다.
	mRoot->bVisualizeComponent = true;

	mArrowComponent = CreateEditorOnlyDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	if (mArrowComponent)
	{
		mArrowComponent->ArrowColor = FColor(150, 200, 255);
		mArrowComponent->bTreatAsASprite = true;
		mArrowComponent->SpriteInfo.Category = TEXT("Spawn");
		mArrowComponent->SpriteInfo.DisplayName =
			NSLOCTEXT("SpriteCategory", "Spawn", "Spawn");
		mArrowComponent->SetupAttachment(mRoot);
		mArrowComponent->bIsScreenSizeScaled = true;
		mArrowComponent->SetSimulatePhysics(false);
	}

#endif
}

// Called when the game starts or when spawned
void AMonsterSpawnPoint::BeginPlay()
{
	Super::BeginPlay();

	// 클라이언트일 경우 제거한다.
	// HasAuthority() : 누가 최종 권한으로 관리하고 있는지 확인한다.
	// true일 경우 서버 false일 경우 클라이언트이다.
	if (!HasAuthority())
	{
		Destroy();
		return;
	}
	
	// 몬스터를 스폰시킨다.
	SpawnMonster();
}

void AMonsterSpawnPoint::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	// 배열을 비워준다.
	//mPatrolPoints.RemoveAll([](FVector Point) {return true; });
	mPatrolPoints.Empty();

	// Splie의 점 수를 얻어온다.
	int32	Count = mPatrolPath->GetNumberOfSplinePoints();

	for (int32 i = 0; i < Count; ++i)
	{
		FVector	Point = mPatrolPath->GetLocationAtSplinePoint(i,
			ESplineCoordinateSpace::World);

		mPatrolPoints.Add(Point);
	}
}

// Called every frame
void AMonsterSpawnPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void AMonsterSpawnPoint::SpawnTimerFinish()
{
	// 타이머를 제거한다.
	GetWorldTimerManager().ClearTimer(mSpawnTimer);

	SpawnMonster();
}

void AMonsterSpawnPoint::SpawnMonster()
{
	// 서버에서만 실행이 되어야 한다.

	GEngine->AddOnScreenDebugMessage(-1, 1000.f, FColor::Blue,
		TEXT("Monster Spawn"));

	if (IsValid(mSpawnClass))
	{
		// Spawn위치를 만들어준다.
		FVector	SpawnLocation = GetActorLocation();

		// Monster CDO를 얻어온다.
		TObjectPtr<AMonsterGAS>	CDO = mSpawnClass->GetDefaultObject<AMonsterGAS>();

		if (IsValid(CDO))
		{
			// CapsuleComponent를 얻어온다.
			//TObjectPtr<UCapsuleComponent>	Capsule =
			//	Cast<UCapsuleComponent>(CDO->GetRootComponent());
			TObjectPtr<UCapsuleComponent>	Capsule = CDO->GetCapsule();

			SpawnLocation.Z += Capsule->GetScaledCapsuleHalfHeight();
		}

		// 몬스터를 스폰시킨다.
		FActorSpawnParameters	param;
		param.SpawnCollisionHandlingOverride =
			ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		mSpawnMonster = GetWorld()->SpawnActor<AMonsterGAS>(mSpawnClass,
			SpawnLocation, GetActorRotation(), param);

		mSpawnMonster->SetSpawnPoint(this);

		mSpawnMonster->SetPatrolPoints(mPatrolPoints);
	}
}
