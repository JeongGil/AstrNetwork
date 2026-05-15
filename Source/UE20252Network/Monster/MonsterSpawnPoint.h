// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "GameFramework/Actor.h"
#include "MonsterSpawnPoint.generated.h"

UCLASS()
class UE20252NETWORK_API AMonsterSpawnPoint : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMonsterSpawnPoint();

protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent>	mRoot;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USplineComponent>	mPatrolPath;

#if WITH_EDITORONLY_DATA
	/** Component shown in the editor only to indicate character facing */
	UPROPERTY()
	TObjectPtr<UArrowComponent> mArrowComponent;
#endif

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AMonsterGAS>	mSpawnClass;

	UPROPERTY(EditAnywhere)
	float			mSpawnTime;

	TObjectPtr<class AMonsterGAS>	mSpawnMonster;

	FTimerHandle		mSpawnTimer;

	UPROPERTY(VisibleAnywhere)
	TArray<FVector>		mPatrolPoints;

public:
	void ClearSpawn()
	{
		if (mSpawnTime > 0.f)
		{
			// 생성된 타이머 핸들은 1번인자로 반환된다.
			// 함수를 지정하고 시간을 지정하여 이 시간 뒤에 지정된 함수가 호출된다.
			// 마지막 false는 반복하지 않는다는 의미이다.
			GetWorldTimerManager().SetTimer(mSpawnTimer, this,
				&AMonsterSpawnPoint::SpawnTimerFinish, mSpawnTime, false);
		}

		else
		{
			SpawnMonster();
		}
	}

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	virtual void OnConstruction(const FTransform& Transform);
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	void SpawnTimerFinish();
	void SpawnMonster();
};
