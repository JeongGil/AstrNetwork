// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterGAS.h"
#include "MonsterAttributeSet.h"
#include "MonsterGASAnimInstance.h"
#include "MonsterGASController.h"
#include "../../Etc/ItemBox.h"
#include "../../GameMode/AssetGameInstanceSubsystem.h"
#include "../../GAS/Effect/GameplayEffect_Gold.h"
#include "../../Player/GAS/PlayerCharacterGAS.h"
#include "Net/UnrealNetwork.h"
#include "UE20252Network/GAS/GameplayAbility_MonsterAttack.h"

AMonsterGAS::AMonsterGAS()
{
	PrimaryActorTick.bCanEverTick = true;

	mBody = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Body"));
	mMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));

	SetRootComponent(mBody);

	mMesh->SetupAttachment(mBody);

	// Capsule이 Navigation에 영향을 주지 않게 세팅한다.
	mBody->SetCanEverAffectNavigation(false);

	mMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	mBody->SetCollisionProfileName(TEXT("Monster"));

	mMovement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("Movement"));

	mMovement->SetUpdatedComponent(mBody);

#if WITH_EDITORONLY_DATA
	mArrowComponent = CreateEditorOnlyDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	if (mArrowComponent)
	{
		mArrowComponent->ArrowColor = FColor(150, 200, 255);
		mArrowComponent->bTreatAsASprite = true;
		mArrowComponent->SpriteInfo.Category = TEXT("Monster");
		mArrowComponent->SpriteInfo.DisplayName =
			NSLOCTEXT("SpriteCategory", "Monster", "Monster");
		mArrowComponent->SetupAttachment(mBody);
		mArrowComponent->bIsScreenSizeScaled = true;
		mArrowComponent->SetSimulatePhysics(false);
	}
#endif // WITH_EDITORONLY_DATA

	AIControllerClass = AMonsterGASController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	SetGenericTeamId(FGenericTeamId(TeamMonster));

	bUseControllerRotationYaw = true;

	mASC = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("ASC"));

	mAttributeSet = CreateDefaultSubobject<UMonsterAttributeSet>(TEXT("AttributeSet"));

	// AbilitySystemComponent가 AttributeSet을 가지고 있게 한다.
	mASC->AddAttributeSetSubobject<UMonsterAttributeSet>(mAttributeSet);
}

void AMonsterGAS::ChangeAnim(EMonsterNormalAnim Anim)
{
	// 이 함수는 서버에서 동작하고 있다.
	// 그렇기 때문에 모든 클라이언트에게 애니메이션을 변경하려면 크게 2가지 방법이 있다.
	// 1. RPC NetMulticast를 이용하여 모든 클라이언트의 함수를 호출하고 변경하게 한다.
	// 2. MonsterGAS 에 멤버변수를 만들고 이 값을 Replicate 한다.
	AnimType = Anim;

	// 서버 AnimInstance의 AnimType을 변경한다.
	if (IsValid(mAnimInst))
	{
		mAnimInst->SetAnim(Anim);
	}
}

void AMonsterGAS::OnRep_ChangeAnimType()
{
	if (HasAuthority())
	{
		return;
	}

	if (IsValid(mAnimInst))
	{
		mAnimInst->SetAnim(AnimType);
	}
}

void AMonsterGAS::OnRep_ChangeHitAlpha()
{
}

void AMonsterGAS::BeginPlay()
{
	Super::BeginPlay();

	// Mesh에 지정되는 Material 수를 얻어온다.
	int32	MaterialCount = mMesh->GetNumMaterials();

	for (int32 i = 0; i < MaterialCount; ++i)
	{
		UMaterialInstanceDynamic* Mtrl = mMesh->CreateDynamicMaterialInstance(i);

		mMaterials.Add(Mtrl);
	}

	mAnimInst = Cast<UMonsterGASAnimInstance>(mMesh->GetAnimInstance());

	if (mDataName.IsNone())
		return;

	UGameInstance* GameInst = GetGameInstance();

	UAssetGameInstanceSubsystem* AssetSubSystem =
		GameInst->GetSubsystem<UAssetGameInstanceSubsystem>();

	if (!AssetSubSystem)
		return;

	// Ability 세팅
	// AbilitySystemComponent에 Owner와 Avatar를 등록한다.
	mASC->InitAbilityActorInfo(this, this);

	mASC->GiveAbility(FGameplayAbilitySpec(UGameplayAbility_MonsterAttack::StaticClass(),
		1, 0));

	if (HasAuthority())
	{
		if (AssetSubSystem->IsMonsterInfoLoaded())
		{
			MonsterInfoLoadComplete();
		}
		else
		{
			AssetSubSystem->mMonsterInfoLoadDelegate.AddUObject(this, &AMonsterGAS::MonsterInfoLoadComplete);
		}
	}
}

void AMonsterGAS::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
}

void AMonsterGAS::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMonsterGAS, AnimType);
	DOREPLIFETIME(AMonsterGAS, HitAlpha);
}

void AMonsterGAS::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void AMonsterGAS::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (mDissolveEnable)
	{
		mDissolve -= 1.5f * DeltaTime / mDissolveTime;

		if (mDissolve <= -1.f)
			Destroy();

		for (auto& Mtrl : mMaterials)
		{
			Mtrl->SetScalarParameterValue(TEXT("DeathDissolve"), mDissolve);
		}
	}
}

float AMonsterGAS::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
	AController* EventInstigator, AActor* DamageCauser)
{
	float	Dmg = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	//Dmg -= mDefense;

	//if (Dmg < 1.f)
	//	Dmg = 1.f;

	//mHP -= Dmg;

	//GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red,
	//	FString::Printf(TEXT("HP : %.2f"), mHP));

	//if (mHP <= 0.f)
	//{
	//	mHP = 0.f;

	//	// 죽는처리
	//	mAnimInst->SetAnim(EMonsterNormalAnim::Death);

	//	// 인공지능을 종료한다.
	//	AMonsterController* MonsterController = GetController<AMonsterController>();

	//	if (IsValid(MonsterController))
	//	{
	//		MonsterController->BrainComponent->StopLogic(TEXT("Death"));
	//		MonsterController->BrainComponent->Cleanup();
	//	}

	//	// 충돌처리를 제거한다.
	//	mBody->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	//	// 이동을 멈춘다.
	//	mMovement->StopMovementImmediately();
	//	// 비활성화.
	//	mMovement->Deactivate();
	//	// 틱 비활성화.
	//	mMovement->SetComponentTickEnabled(false);

	//	return Dmg;
	//}

	return Dmg;
}

void AMonsterGAS::SetGenericTeamId(const FGenericTeamId& TeamID)
{
	mTeamID = TeamID;
}

FGenericTeamId AMonsterGAS::GetGenericTeamId() const
{
	return mTeamID;
}

ETeamAttitude::Type AMonsterGAS::GetTeamAttitudeTowards(const AActor& Other) const
{
	UE_LOG(UELOG, Warning, TEXT("Monster Towards"));

	const IGenericTeamAgentInterface* OtherTeamAgent = Cast<const IGenericTeamAgentInterface>(&Other);

	if (!OtherTeamAgent)
		return ETeamAttitude::Neutral;

	else if (OtherTeamAgent->GetGenericTeamId().GetId() == TeamNeutral)
		return ETeamAttitude::Neutral;

	return GetGenericTeamId() == OtherTeamAgent->GetGenericTeamId() ?
		ETeamAttitude::Friendly : ETeamAttitude::Hostile;
}

void AMonsterGAS::MonsterInfoLoadComplete()
{
	UGameInstance* GameInst = GetGameInstance();

	UAssetGameInstanceSubsystem* AssetSubSystem =
		GameInst->GetSubsystem<UAssetGameInstanceSubsystem>();

	const FMonsterInfo* Info = AssetSubSystem->FindMonsterInfo(mDataName);

	if (!Info)
		return;

	/*mName = Info->MonsterName;
	mLevel = Info->Level;
	mExp = Info->Exp;
	mAttack = Info->Attack;
	mDefense = Info->Defense;
	mHPMax = Info->HPMax;
	mHP = mHPMax;
	mMPMax = Info->MPMax;
	mMP = mMPMax;


	mWalkSpeed = Info->WalkSpeed;
	mRunSpeed = Info->RunSpeed;
	mDetectRange = Info->DetectRange;
	mAttackDistance = Info->AttackDistance;
	mGold = Info->Gold;*/

	if (IsValid(mAttributeSet))
	{
		UE_LOG(UELOG, Warning, TEXT("Attribute Set Complete"));
		mAttributeSet->SetAttack(Info->Attack);
		mAttributeSet->SetDefense(Info->Defense);
		mAttributeSet->SetHP(Info->HPMax);
		mAttributeSet->SetHPMax(Info->HPMax);
		mAttributeSet->SetMP(Info->MPMax);
		mAttributeSet->SetMPMax(Info->MPMax);
		mAttributeSet->SetWalkSpeed(Info->WalkSpeed);
		mAttributeSet->SetRunSpeed(Info->RunSpeed);
		mAttributeSet->SetAttackDistance(Info->AttackDistance);
		mAttributeSet->SetGold(Info->Gold);
		mAttributeSet->SetDetectRange(Info->DetectRange);
	}

	// 이동속도를 지정한다.
	mMovement->MaxSpeed = mAttributeSet->GetWalkSpeed();

	// AI에게 정보를 전달한다.
	TObjectPtr<AMonsterGASController>	AI = GetController<AMonsterGASController>();

	if (IsValid(AI))
	{
		AI->SetDetectRange(mAttributeSet->GetDetectRange());
	}

	const FDropItemTableInfo* DropItemInfo = AssetSubSystem->FindDropItemInfo(mDataName);

	if (!DropItemInfo)
	{
		AssetSubSystem->mDropItemInfoLoadDelegate.AddUObject(this, &AMonsterGAS::DropItemInfoLoadComplete);
	}

	else
	{
		mDropItems = DropItemInfo->DropItems;
	}
}

void AMonsterGAS::DropItemInfoLoadComplete()
{
	UGameInstance* GameInst = GetGameInstance();

	UAssetGameInstanceSubsystem* AssetSubSystem =
		GameInst->GetSubsystem<UAssetGameInstanceSubsystem>();

	const FDropItemTableInfo* Info = AssetSubSystem->FindDropItemInfo(mDataName);

	if (!Info)
		return;

	mDropItems = Info->DropItems;
}

void AMonsterGAS::DetectTarget(bool Detect)
{
	if (Detect)
		mMovement->MaxSpeed = mAttributeSet->GetRunSpeed();

	else
		mMovement->MaxSpeed = mAttributeSet->GetWalkSpeed();
}

void AMonsterGAS::NormalAttack()
{
}

void AMonsterGAS::Death()
{
	mMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	mMesh->SetCollisionProfileName(TEXT("Ragdoll"));

	mMesh->SetSimulatePhysics(true);

	// 물리적인 속도를 0으로 만들어준다.
	mMesh->SetAllPhysicsLinearVelocity(FVector::ZeroVector);
	mMesh->SetAllPhysicsAngularVelocityInDegrees(FVector::ZeroVector);

	// 전체 바디의 물리 시뮬레이션을 켜준다.
	mMesh->SetAllBodiesSimulatePhysics(false);
	mMesh->SetAllBodiesBelowSimulatePhysics(TEXT("pelvis"), true, true);
	// 바디의 Sleep상태를 깨워준다. 비활성화 바디가 모두 활성화가 된다.
	mMesh->WakeAllRigidBodies();
	// 애니메이션 포즈와 물리 결과를 섞어서 반영하도록 한다.
	mMesh->bBlendPhysics = true;

	// Dissolve 활성화
	for (auto& Mtrl : mMaterials)
	{
		Mtrl->SetScalarParameterValue(TEXT("DeathDissolveEnable"), 1.f);
	}

	mDissolveEnable = true;

	//SetLifeSpan(3.f);

	UE_LOG(UELOG, Warning, TEXT("Monster End : %d"), mDropItems.Num());

	// Item Drop확률을 계산한다.
	for (auto Item : mDropItems)
	{
		float Percent = FMath::FRand();
		// RoundToFloat은 소수점 자리를 가장 가까운 정수 값으로 반올림해서 float으로 반환하는
		// 함수이다.
		Percent = FMath::RoundToFloat(Percent * 10000.f) / 100.f;

		UE_LOG(UELOG, Warning, TEXT("Percent : %.4f, Item Percent : %.4f"), Percent,
			Item.Percent);

		if (Percent <= Item.Percent)
		{
			float	RandLocX = FMath::FRandRange(-150.f, 150.f);
			float	RandLocY = FMath::FRandRange(-150.f, 150.f);

			// 아이템 상자를 스폰시킨다.
			FActorSpawnParameters	param;
			param.SpawnCollisionHandlingOverride =
				ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

			AItemBox* ItemBox = GetWorld()->SpawnActor<AItemBox>(
				GetActorLocation() + FVector(RandLocX, RandLocY, 0.0),
				GetActorRotation(), param);

			ItemBox->SetItem(Item.ItemKey);
		}
	}
}

void AMonsterGAS::CallbackHP(AActor* InstigatorActor)
{
	// 죽었는지 체크한다.
	if (mAttributeSet->GetHP() <= 0.f)
	{
		UE_LOG(UELOG, Warning, TEXT("Death"));

		// 죽는처리
		if (IsValid(mAnimInst))
		{
			mAnimInst->SetAnim(EMonsterNormalAnim::Death);
		}

		APlayerCharacterGAS* PlayerCharacter = Cast<APlayerCharacterGAS>(InstigatorActor);

		if (IsValid(PlayerCharacter))
		{
			UAbilitySystemComponent* PlayerASC = PlayerCharacter->GetAbilitySystemComponent();

			// Gold 증가용 GameplayEffect를 이용하여 SetByCaller로 증가할 골드를 지정하고
			// GameplayEffect를 동작시켜준다.
			FGameplayEffectContextHandle	ContextHandle = PlayerASC->MakeEffectContext();

			ContextHandle.AddSourceObject(this);

			FGameplayEffectSpecHandle	SpecHandle = PlayerASC->MakeOutgoingSpec(
				UGameplayEffect_Gold::StaticClass(), 1, ContextHandle);

			if (SpecHandle.IsValid())
			{
				SpecHandle.Data->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(TEXT("Effect.Gold")), mAttributeSet->GetGold());

				PlayerASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data);
			}
		}

		// 인공지능을 종료한다.
		AMonsterGASController* MonsterController = GetController<AMonsterGASController>();

		if (IsValid(MonsterController))
		{
			MonsterController->BrainComponent->StopLogic(TEXT("Death"));
			MonsterController->BrainComponent->Cleanup();
		}

		// 충돌처리를 제거한다.
		mBody->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		// 이동을 멈춘다.
		mMovement->StopMovementImmediately();
		// 비활성화.
		mMovement->Deactivate();
		// 틱 비활성화.
		mMovement->SetComponentTickEnabled(false);
	}

	else
	{
		if (!mHit)
		{
			mHit = true;

			if (IsValid(mAnimInst))
			{
				mAnimInst->Hit();
			}

			GetWorldTimerManager().SetTimer(mHitTimer, this, &AMonsterGAS::HitEnd,
				0.1f, false);

			for (auto& Mtrl : mMaterials)
			{
				Mtrl->SetScalarParameterValue(TEXT("Hit"), 1.f);
			}
		}
	}
}

void AMonsterGAS::HitEnd()
{
	GetWorldTimerManager().ClearTimer(mHitTimer);

	mHit = false;

	for (auto& Mtrl : mMaterials)
	{
		Mtrl->SetScalarParameterValue(TEXT("Hit"), 0.f);
	}
}
