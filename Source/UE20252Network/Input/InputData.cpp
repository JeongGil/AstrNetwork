// Fill out your copyright notice in the Description page of Project Settings.


#include "InputData.h"

UInputData::UInputData()
{
}

TObjectPtr<UInputAction> UInputData::FindAction(const FString& Name)	const
{
	return mActions.FindRef(Name);
}


// CDO가 기본으로 생성되며 입력 에셋들은 모두 로딩이 되어 있는 상태이므로
// 필요하면 CDO를 이용해서 이 에셋에 접근해서 사용한다.
UDefaultInputData::UDefaultInputData()
{
	static ConstructorHelpers::FObjectFinder<UInputMappingContext>	InputContext(TEXT("/Script/EnhancedInput.InputMappingContext'/Game/Input/IMC_Default.IMC_Default'"));

	if (InputContext.Succeeded())
		mContext = InputContext.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction>	MoveAction(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/IA_Move.IA_Move'"));

	if (MoveAction.Succeeded())
		mActions.Add(TEXT("Move"), MoveAction.Object);


	static ConstructorHelpers::FObjectFinder<UInputAction>	RotationAction(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/IA_Rotation.IA_Rotation'"));

	if (RotationAction.Succeeded())
		mActions.Add(TEXT("Rotation"), RotationAction.Object);


	static ConstructorHelpers::FObjectFinder<UInputAction>	JumpAction(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/IA_Jump.IA_Jump'"));

	if (JumpAction.Succeeded())
		mActions.Add(TEXT("Jump"), JumpAction.Object);


	static ConstructorHelpers::FObjectFinder<UInputAction>	AttackAction(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/IA_Attack.IA_Attack'"));

	if (AttackAction.Succeeded())
		mActions.Add(TEXT("Attack"), AttackAction.Object);


	static ConstructorHelpers::FObjectFinder<UInputAction>	Skill1Action(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/IA_Skill1.IA_Skill1'"));

	if (Skill1Action.Succeeded())
		mActions.Add(TEXT("Skill1"), Skill1Action.Object);


	static ConstructorHelpers::FObjectFinder<UInputAction>	Skill2Action(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/IA_Skill2.IA_Skill2'"));

	if (Skill2Action.Succeeded())
		mActions.Add(TEXT("Skill2"), Skill2Action.Object);


	static ConstructorHelpers::FObjectFinder<UInputAction>	Skill3Action(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/IA_Skill3.IA_Skill3'"));

	if (Skill3Action.Succeeded())
		mActions.Add(TEXT("Skill3"), Skill3Action.Object);


	static ConstructorHelpers::FObjectFinder<UInputAction>	ColorAction(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/IA_ColorChange.IA_ColorChange'"));

	if (ColorAction.Succeeded())
		mActions.Add(TEXT("ColorChange"), ColorAction.Object);
}

USelectInputData::USelectInputData()
{
	static ConstructorHelpers::FObjectFinder<UInputMappingContext>	InputContext(TEXT("/Script/EnhancedInput.InputMappingContext'/Game/Input/IMC_Select.IMC_Select'"));

	if (InputContext.Succeeded())
		mContext = InputContext.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction>	ClickAction(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/IA_Click.IA_Click'"));

	if (ClickAction.Succeeded())
		mActions.Add(TEXT("Click"), ClickAction.Object);
}
