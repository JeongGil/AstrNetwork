// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../GameInfo.h"
#include "GameFramework/Pawn.h"
#include "MonsterGAS.generated.h"

UCLASS()
class UE20252NETWORK_API AMonsterGAS : public APawn,
	public IGenericTeamAgentInterface,
	public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AMonsterGAS();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCapsuleComponent> mBody;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> mMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UFloatingPawnMovement> mMovement;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FName		mDataName;

	FGenericTeamId		mTeamID;

#if WITH_EDITORONLY_DATA
	/** Component shown in the editor only to indicate character facing */
	UPROPERTY()
	TObjectPtr<UArrowComponent> mArrowComponent;
#endif

	TObjectPtr<class AMonsterSpawnPoint>	mSpawnPoint;

	UPROPERTY(VisibleAnywhere)
	TArray<FVector>		mPatrolPoints;

	int32				mPatrolIndex = 1;

	TObjectPtr<class UMonsterGASAnimInstance>	mAnimInst;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAbilitySystemComponent>	mASC;

	TObjectPtr<class UMonsterAttributeSet>	mAttributeSet;

	TArray<FDropItem>		mDropItems;

	TArray<TObjectPtr<UMaterialInstanceDynamic>>	mMaterials;

	bool					mHit = false;

	FTimerHandle			mHitTimer;

	bool					mDissolveEnable = false;
	float					mDissolve = 0.5f;
	float					mDissolveTime = 3.f;

	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_ChangeAnimType)
	EMonsterNormalAnim AnimType;

	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_ChangeHitAlpha)
	float HitAlpha;

public:
	class UMonsterAttributeSet* GetAttributeSet()	const
	{
		return mAttributeSet;
	}

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const
	{
		return mASC;
	}

	bool GetPatrolEnable()	const
	{
		return mPatrolPoints.Num() > 1;
	}

	FVector GetPatrolPoint()	const
	{
		return mPatrolPoints[mPatrolIndex];
	}

	void NextPatrol()
	{
		mPatrolIndex = (mPatrolIndex + 1) % mPatrolPoints.Num();
	}

	UCapsuleComponent* GetCapsule()	const
	{
		return mBody;
	}

	void SetSpawnPoint(class AMonsterSpawnPoint* Point)
	{
		mSpawnPoint = Point;
	}

	void SetPatrolPoints(const TArray<FVector>& Points)
	{
		mPatrolPoints = Points;
	}

	void ChangeAnim(EMonsterNormalAnim Anim);

	UFUNCTION()
	void OnRep_ChangeAnimType();

	UFUNCTION()
	void OnRep_ChangeHitAlpha();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason);

public:
	virtual void OnConstruction(const FTransform& Transform);
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void Tick(float DeltaTime) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser);

public:
	virtual void SetGenericTeamId(const FGenericTeamId& TeamID);
	virtual FGenericTeamId GetGenericTeamId() const;
	virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const;

public:
	UFUNCTION()
	void MonsterInfoLoadComplete();

	UFUNCTION()
	void DropItemInfoLoadComplete();

	void DetectTarget(bool Detect);

public:
	virtual void NormalAttack();
	virtual void Death();
	virtual void CallbackHP(AActor* InstigatorActor);

public:
	void HitEnd();
};
