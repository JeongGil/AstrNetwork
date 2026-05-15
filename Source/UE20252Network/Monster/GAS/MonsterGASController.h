// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../GameInfo.h"
#include "AIController.h"
#include "MonsterGASController.generated.h"

/**
 * 
 */
UCLASS()
class UE20252NETWORK_API AMonsterGASController : public AAIController
{
	GENERATED_BODY()

public:
	AMonsterGASController();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAIPerceptionComponent>	mAIPerception;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAISenseConfig_Sight>	mSightConfig;

	UPROPERTY()
	TObjectPtr<UBehaviorTree>	mAITree;

	UPROPERTY()
	TSoftObjectPtr<UBehaviorTree>	mAITreeLoader;

protected:
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* aPawn) override;
	virtual void OnUnPossess() override;

public:
	virtual void OnConstruction(const FTransform& Transform);
	virtual void Tick(float DeltaTime) override;

public:
	void SetAITree(const FString& Path);
	void SetAITree(UBehaviorTree* AITree);
	void SetDetectRange(float Range);

	UFUNCTION()
	void AILoadComplete(const FSoftObjectPath& AssetPath, UObject* LoadObject);

	// 타겟을 감지했을때 호출될 함수
	UFUNCTION()
	void OnTarget(AActor* Actor, FAIStimulus Stimulus);

public:
	virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const;
};