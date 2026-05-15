// Fill out your copyright notice in the Description page of Project Settings.


#include "GeometryActor.h"

// Sets default values
AGeometryActor::AGeometryActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	mGeometry = CreateDefaultSubobject<UGeometryCollectionComponent>(TEXT("Geometry"));

	SetRootComponent(mGeometry);

	//FJsonSerializableArrayFloat
	//mGeometry->SetDamageThreshold()
}

// Called when the game starts or when spawned
void AGeometryActor::BeginPlay()
{
	Super::BeginPlay();

	mGeometry->OnComponentHit.AddDynamic(this, &AGeometryActor::GeometryHit);
}

// Called every frame
void AGeometryActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGeometryActor::SetGeometryAsset(const FString& Path)
{
	TObjectPtr<UGeometryCollection> Geometry = LoadObject<UGeometryCollection>(
		nullptr, Path);

	mGeometry->SetRestCollection(Geometry);
}

void AGeometryActor::GeometryHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, TEXT("GeometryHit"));

	int32	ItemIndex = 0;
	
	if (Hit.Item != -1)
	{
		ItemIndex = Hit.Item;
	}

	UE_LOG(UELOG, Warning, TEXT("Index : %d"), ItemIndex);

	mGeometry->ApplyExternalStrain(
		ItemIndex,			// 맞은 조각. 이 조각에 힘을 가한다.
		Hit.ImpactPoint,	// 파괴 위치. 힘을 가할 위치이다.
		50.f,				// 반경. 얼마나 넓은 범위까지 영향을 줄지 결정한다. 크면 클수록 주변 조각에 영향을 준다.
		1,		// 전파될 레벨 깊이.
		1.f,		// 전파 비율. 힘이 그대로 유지될지, 약해지게 할지. 0은 거의 퍼지지 않음.
		1500000.f	// 조각을 깨트리는 힘의 크기
	);
}
