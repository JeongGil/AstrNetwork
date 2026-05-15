// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterGASController.h"
#include "MonsterGAS.h"

AMonsterGASController::AMonsterGASController()
{
	PrimaryActorTick.bCanEverTick = true;

	mAIPerception = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception"));

	SetPerceptionComponent(*mAIPerception);

	// 시야를 만들어준다.
	mSightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight"));

	// 시야 거리.
	mSightConfig->SightRadius = 800.f;

	// 시야를 잃는 거리.
	mSightConfig->LoseSightRadius = 800.f;

	// 탐지 각도.
	mSightConfig->PeripheralVisionAngleDegrees = 180.f;

	// 탐지할 종류를 정한다.
	mSightConfig->DetectionByAffiliation.bDetectEnemies = true;

	mSightConfig->DetectionByAffiliation.bDetectNeutrals = true;

	mSightConfig->DetectionByAffiliation.bDetectFriendlies = false;

	// PerceptionComponent에 감각을 등록한다.
	mAIPerception->ConfigureSense(*mSightConfig);

	// 주요 감각을 지정한다.
	mAIPerception->SetDominantSense(mSightConfig->GetSenseImplementation());

	// 팀을 등록한다.
	SetGenericTeamId(FGenericTeamId(TeamMonster));

	mAIPerception->OnTargetPerceptionUpdated.AddDynamic(this,
		&AMonsterGASController::OnTarget);
}

void AMonsterGASController::BeginPlay()
{
	Super::BeginPlay();

	// 시작할 때 강제로 주변을 탐색하게 갱신한다.
	mAIPerception->RequestStimuliListenerUpdate();
}

void AMonsterGASController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);
}

void AMonsterGASController::OnUnPossess()
{
	Super::OnUnPossess();
}

void AMonsterGASController::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
}

void AMonsterGASController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMonsterGASController::SetAITree(const FString& Path)
{
	FSoftObjectPath	SoftPath(Path);

	mAITreeLoader = TSoftObjectPtr<UBehaviorTree>(SoftPath);

	if (!mAITreeLoader.IsNull())
	{
		//mAITreeLoader.LoadAsync(FLoadSoftObjectPathAsyncDelegate::CreateUObject(this,
		//	&AMonsterGASController::AILoadComplete));

		mAITree = mAITreeLoader.LoadSynchronous();

		if (!IsValid(mAITree))
		{
			UE_LOG(UELOG, Warning, TEXT("BehaviorTree Load Failed"));
			return;
		}

		if (!RunBehaviorTree(mAITree))
		{
			UE_LOG(UELOG, Warning, TEXT("BehaviorTree Run Failed"));
			return;
		}

		UE_LOG(UELOG, Warning, TEXT("BehaviorTree Run Success"));
	}

	/*mAITree = mAITreeLoader.LoadSynchronous();
	RunBehaviorTree(mAITree);
	mAITreeLoader.LoadAsync(FLoadSoftObjectPathAsyncDelegate::CreateUObject(this,
		&AMonsterGASController::AILoadComplete));*/
}

void AMonsterGASController::SetAITree(UBehaviorTree* AITree)
{
	mAITree = AITree;
}

void AMonsterGASController::SetDetectRange(float Range)
{
	mSightConfig->SightRadius = Range;

	// 시야를 잃는 거리.
	mSightConfig->LoseSightRadius = Range + 200.f;

	mAIPerception->ConfigureSense(*mSightConfig);
}

void AMonsterGASController::AILoadComplete(const FSoftObjectPath& AssetPath,
	UObject* LoadObject)
{
	mAITree = Cast<UBehaviorTree>(LoadObject);

	if (!IsValid(mAITree))
	{
		UE_LOG(UELOG, Warning, TEXT("AI Load Failed"));
	}

	if (!RunBehaviorTree(mAITree))
	{
		UE_LOG(UELOG, Warning, TEXT("AI Run Failed"));
	}

	UE_LOG(UELOG, Warning, TEXT("AI Load Success"));
}

ETeamAttitude::Type AMonsterGASController::GetTeamAttitudeTowards(const AActor& Other) const
{
	UE_LOG(UELOG, Warning, TEXT("AIController Towards"));

	const IGenericTeamAgentInterface* OtherTeamAgent = Cast<const IGenericTeamAgentInterface>(&Other);

	if (!OtherTeamAgent)
		return ETeamAttitude::Neutral;

	else if (OtherTeamAgent->GetGenericTeamId().GetId() == TeamNeutral)
		return ETeamAttitude::Neutral;

	return GetGenericTeamId() == OtherTeamAgent->GetGenericTeamId() ?
		ETeamAttitude::Friendly : ETeamAttitude::Hostile;
}

void AMonsterGASController::OnTarget(AActor* Actor, FAIStimulus Stimulus)
{
	if (!IsValid(mAITree) || !Blackboard)
		return;

	AMonsterGAS* Monster = GetPawn<AMonsterGAS>();

	if (!IsValid(Monster))
		return;

	// 감지 성공
	if (Stimulus.WasSuccessfullySensed())
	{
		// BlackboardComponent를 이용해서 Blackboard에 선언한 변수의 값을 변경할 수 있다.
		Blackboard->SetValueAsObject(TEXT("Target"), Actor);

		Monster->DetectTarget(true);

		UE_LOG(UELOG, Warning, TEXT("Target Detect : %s"),
			*Actor->GetName());
	}

	// 감지 실패
	else
	{
		Blackboard->SetValueAsObject(TEXT("Target"), nullptr);

		Monster->DetectTarget(false);

		UE_LOG(UELOG, Warning, TEXT("Target Detect Failed"));
	}
}
