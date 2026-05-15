// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacterGAS.h"
#include "../../Input/InputData.h"
#include "../PlayerAnimInstance.h"
#include "../MainPlayerState.h"
#include "PlayerAttributeSet.h"
#include "../../GAS/GameplayAbility_Attack.h"
#include "../MainPlayerController.h"
#include "../../UI/Main/MainWidget.h"
#include "../../UI/Main/PlayerInfoWidget.h"

// Sets default values
APlayerCharacterGAS::APlayerCharacterGAS()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Component를 생성해서 멤버변수에 주소를 저장한다.
	// CreateDefaultSubobject 함수는 생성자에서 UObject 객체를 생성할 때 사용한다.
	mSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Arm"));

	// SpringArm의 부모 컴포넌트로 Mesh를 지정한다.
	mSpringArm->SetupAttachment(GetMesh());

	mSpringArm->TargetArmLength = 200.f;
	//mSpringArm->SocketOffset.Y = 20.0;
	mSpringArm->SetRelativeLocation(FVector(0.0, 0.0, 150.0));
	mSpringArm->SetRelativeRotation(FRotator(-10.0, 90.0, 0.0));

	// CameraComponent를 생성하고 SpringArm에 붙여준다.
	mCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));

	mCamera->SetupAttachment(mSpringArm);

	mHeadUpWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("HeadUpWidget"));

	mHeadUpWidget->SetupAttachment(GetMesh());

	/*static ConstructorHelpers::FClassFinder<UPlayerInfoWidget>	InfoClass(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UI/Main/UB_PlayerInfo.UB_PlayerInfo_C'"));

	if (InfoClass.Succeeded())
		mHeadUpWidget->SetWidgetClass(InfoClass.Class);

	mHeadUpWidget->SetWidgetSpace(EWidgetSpace::Screen);
	mHeadUpWidget->SetDrawSize(FVector2D(200.0, 80.0));
	mHeadUpWidget->SetCollisionEnabled(ECollisionEnabled::NoCollision);*/


	bUseControllerRotationYaw = true;

	GetCharacterMovement()->JumpZVelocity = 700.f;

	// Mesh는 충돌을 제외한다.
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// Damage를 받을지 여부이다. false일 경우 무적 상태를 만든다.
	// false라면 Actor클래스의 TakeDamage에서 데미지를 0으로 반환한다.
	//SetCanBeDamaged(true);

	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Player"));

	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	SetGenericTeamId(FGenericTeamId(TeamPlayer));


	mASC = CreateDefaultSubobject<UPlayerAbilitySystemComponent>(TEXT("ASC"));

	mAttributeSet = CreateDefaultSubobject<UPlayerAttributeSet>(TEXT("AttributeSet"));

	// AbilitySystemComponent가 AttributeSet을 가지고 있게 한다.
	mASC->AddAttributeSetSubobject<UPlayerAttributeSet>(mAttributeSet);

	mASC->SetIsReplicated(true);

	SetReplicates(true);
	SetReplicateMovement(true);

	//mInventoryCom = CreateDefaultSubobject<UInventoryComponent>(TEXT("Inventory"));

	/*static ConstructorHelpers::FClassFinder<UCameraShakeBase>	DefaultShake(TEXT("/Script/Engine.Blueprint'/Game/Blueprints/BPLGShake.BPLGShake_C'"));

	if (DefaultShake.Succeeded())
		mDefaultShake = DefaultShake.Class;*/
}

// Called when the game starts or when spawned
void APlayerCharacterGAS::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(UELOG, Warning, TEXT("PlayerCharacterGAS BeginPlay"));

	// 플레이어 정보를 설정한다.
	//GetPlayerState<AMainPlayerState>()->LoadPlayerInfo(mPlayerName);

	mAnimInst = Cast<UPlayerAnimInstance>(GetMesh()->GetAnimInstance());

	// PlayerController를 얻어온다.
	TObjectPtr<APlayerController>	PlayerController = GetController<APlayerController>();

	// IsValid : 유효성 검사를 해주는 함수이다. 언리얼 객체가 유효한지를 판단해준다.
	if (IsValid(PlayerController))
	{
		// Enhanced Input System을 얻어온다.
		TObjectPtr<UEnhancedInputLocalPlayerSubsystem>	Subsystem =
			ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());

		// UDefaultInputData CDO를 얻어온다.
		const UDefaultInputData* InputData = GetDefault<UDefaultInputData>();

		// MappingContext를 등록한다.
		Subsystem->AddMappingContext(InputData->mContext, 0);
	}

	//mInfoWidget = Cast<UPlayerInfoWidget>(mHeadUpWidget->GetWidget());

	//mInfoWidget->SetPlayerName

	// Ability 세팅
	// AbilitySystemComponent에 Owner와 Avatar를 등록한다.
	mASC->InitAbilityActorInfo(this, this);

	mASC->GiveAbility(FGameplayAbilitySpec(UGameplayAbility_Attack::StaticClass(),
		1, 0));

	mASC->GetGameplayAttributeValueChangeDelegate(UBaseAttributeSet::GetHPAttribute()).AddUObject(this, &APlayerCharacterGAS::OnHPChange);

	mASC->GetGameplayAttributeValueChangeDelegate(UBaseAttributeSet::GetMPAttribute()).AddUObject(this, &APlayerCharacterGAS::OnMPChange);

	mASC->GetGameplayAttributeValueChangeDelegate(UBaseAttributeSet::GetGoldAttribute()).AddUObject(this, &APlayerCharacterGAS::OnGoldChange);
}

// Called every frame
void APlayerCharacterGAS::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APlayerCharacterGAS::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// 인자로 들어온 InputComponent를 EnhancedInputComponent로 형변환한다.
	// 언리얼 오브젝트는 항상 Cast<Type>() 함수를 이용해서 형변환한다.
	TObjectPtr<UEnhancedInputComponent>	Input =
		Cast<UEnhancedInputComponent>(PlayerInputComponent);

	if (IsValid(Input))
	{
		// UDefaultInputData CDO를 얻어온다.
		const UDefaultInputData* InputData = GetDefault<UDefaultInputData>();

		// 이동키를 누를때 동작할 함수를 바인딩한다.
		Input->BindAction(InputData->FindAction(TEXT("Move")), ETriggerEvent::Triggered,
			this, &APlayerCharacterGAS::MoveKey);

		Input->BindAction(InputData->FindAction(TEXT("Rotation")), ETriggerEvent::Triggered,
			this, &APlayerCharacterGAS::RotationKey);

		Input->BindAction(InputData->FindAction(TEXT("Jump")), ETriggerEvent::Started,
			this, &APlayerCharacterGAS::JumpKey);

		Input->BindAction(InputData->FindAction(TEXT("Attack")), ETriggerEvent::Started,
			this, &APlayerCharacterGAS::AttackKey);

		Input->BindAction(InputData->FindAction(TEXT("Skill1")), ETriggerEvent::Started,
			this, &APlayerCharacterGAS::Skill1Key);

		Input->BindAction(InputData->FindAction(TEXT("Skill2")), ETriggerEvent::Started,
			this, &APlayerCharacterGAS::Skill2Key);

		Input->BindAction(InputData->FindAction(TEXT("Skill2")), ETriggerEvent::Completed,
			this, &APlayerCharacterGAS::Skill2ReleasedKey);

		Input->BindAction(InputData->FindAction(TEXT("Skill3")), ETriggerEvent::Started,
			this, &APlayerCharacterGAS::Skill3Key);

		Input->BindAction(InputData->FindAction(TEXT("ColorChange")),
			ETriggerEvent::Completed,
			this, &APlayerCharacterGAS::ColorChangeKey);
	}
}

float APlayerCharacterGAS::TakeDamage(float DamageAmount,
	struct FDamageEvent const& DamageEvent, class AController* EventInstigator,
	AActor* DamageCauser)
{
	float Dmg = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator,
		DamageCauser);

	if (Dmg == 0.f)
		return 0.f;

	Dmg -= GetPlayerState<AMainPlayerState>()->GetDefense();

	if (Dmg < 1.f)
		Dmg = 1.f;

	if (GetPlayerState<AMainPlayerState>()->AddHP(-Dmg))
	{
		// 죽었을 때 처리
	}

	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red,
		FString::Printf(TEXT("Dmg : %.5f"), Dmg));

	//CanBeDamaged()

	return Dmg;
}

void APlayerCharacterGAS::MoveKey(const FInputActionValue& Value)
{
	FVector	Axis = Value.Get<FVector>();

	AddMovementInput(GetActorForwardVector(), Axis.X);

	AddControllerYawInput(Axis.Y);

	/*FVector Forward = GetActorForwardVector();
	FVector Right = GetActorRightVector();

	FVector	Dir = Forward * Axis.X + Right * Axis.Y;
	Dir.Normalize();

	AddMovementInput(Dir);*/
}

void APlayerCharacterGAS::RotationKey(const FInputActionValue& Value)
{
	FVector	Axis = Value.Get<FVector>();

	//AddControllerYawInput(Axis.X);
	mSpringArm->AddRelativeRotation(FRotator(Axis.Y, Axis.X, 0.0));

	mAnimInst->AddViewPitch(Axis.Y);
	mAnimInst->AddViewYaw(Axis.X);
}

void APlayerCharacterGAS::JumpKey(const FInputActionValue& Value)
{
	if (CanJump())
		Jump();
}

void APlayerCharacterGAS::AttackKey(const FInputActionValue& Value)
{
	// 소모할 마나를 지정한다.
	mASC->SetSkillMana(0.f);

	InputAttack();
	//UE_LOG(UELOG, Warning, TEXT("AttackKey"));

	//GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, TEXT("AttackKey"));

	//// 총알을 만들어줄 위치와 회전을 구한다.
	//FVector	SpawnLoc = GetActorLocation() + GetActorForwardVector() * 150.f;

	//FActorSpawnParameters	param;

	//param.SpawnCollisionHandlingOverride =
	//	ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	//// GetWorld() 함수는 World의 객체를 얻어온다.
	//TObjectPtr<ATestBullet>	Bullet =
	//	GetWorld()->SpawnActor<ATestBullet>(SpawnLoc, GetActorRotation(), param);

	//Bullet->SetLifeSpan(5.f);
	//// 태그 추가
	//Bullet->Tags.Add(TEXT("PlayerBullet"));
}

void APlayerCharacterGAS::Skill1Key(const FInputActionValue& Value)
{
	//Skill1();
	APlayerController* PC = GetController<APlayerController>();

	/*PC->PlayerCameraManager->StartCameraShake(UDefaultCameraShake::StaticClass(),
		1.f, ECameraShakePlaySpace::CameraLocal);*/
	/*PC->PlayerCameraManager->StartCameraShake(mDefaultShake,
		1.f, ECameraShakePlaySpace::CameraLocal);*/
}

void APlayerCharacterGAS::Skill2Key(const FInputActionValue& Value)
{
	//Skill2();
	// 소모할 마나를 지정한다.
	mASC->SetSkillMana(30.f);

	// 현재 쿨타임중인 스킬인지 판단한다.
	if (!mASC->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(TEXT("Effect.Skill.CoolDown2"))))
	{
		mASC->AbilityLocalInputPressed((int32)EAbilityInputID::Skill2);
	}

	else
	{
		UE_LOG(UELOG, Warning, TEXT("Skill2번은 쿨타임이 진행중입니다."));
	}

	// 쿨타임 남은시간 확인
	FGameplayEffectQuery	Query = FGameplayEffectQuery::MakeQuery_MatchAnyOwningTags(
		FGameplayTagContainer(FGameplayTag::RequestGameplayTag(TEXT("Effect.Skill.CoolDown2"))));

	TArray<float>	Times = mASC->GetActiveEffectsTimeRemaining(Query);

	if (Times.Num() > 0)
	{
		UE_LOG(UELOG, Warning, TEXT("Skill Time : %.2f"), Times[0]);
	}
}

void APlayerCharacterGAS::Skill2ReleasedKey(const FInputActionValue& Value)
{
	mASC->AbilityLocalInputReleased((int32)EAbilityInputID::Skill2);
}

void APlayerCharacterGAS::Skill3Key(const FInputActionValue& Value)
{
	// Ghost 스킬 사용.
	Skill3();
}

void APlayerCharacterGAS::ColorChangeKey(const FInputActionValue& Value)
{
	/*ATestMaterialDN* Actor = Cast<ATestMaterialDN>(UGameplayStatics::GetActorOfClass(
		GetWorld(), ATestMaterialDN::StaticClass()));

	if (IsValid(Actor))
	{
		Actor->ChangeColor();
	}*/
}

void APlayerCharacterGAS::InputAttack()
{
}

void APlayerCharacterGAS::Skill1()
{
}

void APlayerCharacterGAS::Skill2()
{
}

void APlayerCharacterGAS::Skill3()
{
}

void APlayerCharacterGAS::NormalAttack()
{
}

void APlayerCharacterGAS::Skill1Casting()
{
}

void APlayerCharacterGAS::OnHPChange(const FOnAttributeChangeData& HP)
{
	float NewValue = HP.NewValue;
	float OldValue = HP.OldValue;

	float Percent = NewValue / mAttributeSet->GetHPMax();

	AMainPlayerController* PC = GetController<AMainPlayerController>();

	if (IsValid(PC))
	{
		//PC->GetMainWidget()->GetPlayerHUDWidget()->SetHPPercent(Percent);
	}

	//mInfoWidget->SetHPPercent(Percent);
}

void APlayerCharacterGAS::OnMPChange(const FOnAttributeChangeData& MP)
{
	float NewValue = MP.NewValue;
	float OldValue = MP.OldValue;

	float Percent = NewValue / mAttributeSet->GetMPMax();

	AMainPlayerController* PC = GetController<AMainPlayerController>();

	if (IsValid(PC))
	{
		//PC->GetMainWidget()->GetPlayerHUDWidget()->SetMPPercent(Percent);
	}
}

void APlayerCharacterGAS::OnGoldChange(const FOnAttributeChangeData& Gold)
{
	//mInventoryCom->ChangeGold(Gold.NewValue);
}

void APlayerCharacterGAS::Acquisition(const FItemTableInfo* ItemInfo)
{
	//mInventoryCom->AddItem(ItemInfo);
}

void APlayerCharacterGAS::EquipWeapon(UStaticMesh* WeaponMesh)
{
	if (!WeaponMesh || !GetMesh())
		return;

	// 기존 무기 제거
	//UnEquipWeapon();

	//// 무기를 장착할 Component를 생성한다.
	//mEquipWeapon = NewObject<UStaticMeshComponent>(this);

	//// 월드에서 동작 가능하게 등록한다.
	//mEquipWeapon->RegisterComponent();

	//// 무기 Mesh를 지정한다.
	//mEquipWeapon->SetStaticMesh(WeaponMesh);

	//// SnapToTargetIncludingScale : 부모 소켓의 위치와 회전은 그대로 따라가고 스케일은
	//// 따라가지 않는다.
	//mEquipWeapon->AttachToComponent(GetMesh(),
	//	FAttachmentTransformRules::SnapToTargetIncludingScale,
	//	TEXT("weapon_l_socket"));

	//mEquipWeapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void APlayerCharacterGAS::UnEquipWeapon()
{
	/*if (mEquipWeapon)
	{
		mEquipWeapon->DestroyComponent();
		mEquipWeapon = nullptr;
	}*/
}

void APlayerCharacterGAS::SetGenericTeamId(const FGenericTeamId& TeamID)
{
	mTeamID = TeamID;
}

FGenericTeamId APlayerCharacterGAS::GetGenericTeamId() const
{
	return mTeamID;
}

ETeamAttitude::Type APlayerCharacterGAS::GetTeamAttitudeTowards(const AActor& Other) const
{
	UE_LOG(UELOG, Warning, TEXT("Player Towards"));

	const IGenericTeamAgentInterface* OtherTeamAgent = Cast<const IGenericTeamAgentInterface>(&Other);

	if (!OtherTeamAgent)
		return ETeamAttitude::Neutral;

	else if (OtherTeamAgent->GetGenericTeamId().GetId() == TeamNeutral)
		return ETeamAttitude::Neutral;

	return GetGenericTeamId() == OtherTeamAgent->GetGenericTeamId() ?
		ETeamAttitude::Friendly : ETeamAttitude::Hostile;
}
