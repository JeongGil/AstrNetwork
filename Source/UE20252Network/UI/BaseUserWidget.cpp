// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseUserWidget.h"
#include "../GameMode/UIGameInstanceSubsystem.h"

UBaseUserWidget::UBaseUserWidget(const FObjectInitializer& ObjectInitializer)	:
	Super(ObjectInitializer)
{
}

void UBaseUserWidget::PlayWidgetAnimation(const FString& Name, float PlayStartTime,
	float PlaySpeed, bool Forward, bool RestoreState, int32 LoopCount)
{
	TObjectPtr<UWidgetAnimation>	Anim = mAnimMap.FindRef(Name);

	if (IsValid(Anim))
	{
		EUMGSequencePlayMode::Type	PlayMode = EUMGSequencePlayMode::Forward;

		if (!Forward)
			PlayMode = EUMGSequencePlayMode::Reverse;

		PlayAnimation(Anim, PlayStartTime, LoopCount, PlayMode, PlaySpeed, RestoreState);
	}
}

void UBaseUserWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	UGameInstance* GameInst = GetGameInstance();

	UUIGameInstanceSubsystem* UISubSystem =
		GameInst->GetSubsystem<UUIGameInstanceSubsystem>();

	UISubSystem->AddWidget(mWidgetName, this);

	UWidgetBlueprintGeneratedClass* GeneratedClass = GetWidgetTreeOwningClass();

	for (auto& Anim : GeneratedClass->Animations)
	{
		FString	Name = Anim->GetName();

		// 애니메이션 이름 뒤에 _INST가 붙어있기 때문에 제거한다.
		Name.ReplaceInline(TEXT("_INST"), TEXT(""));

		mAnimMap.Add(Name, Anim);
	}
}

void UBaseUserWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	UE_LOG(UELOG, Warning, TEXT("NativePreConstruct"));
}

void UBaseUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	UE_LOG(UELOG, Warning, TEXT("NativeConstruct"));
}

void UBaseUserWidget::NativeDestruct()
{
	Super::NativeDestruct();

	UGameInstance* GameInst = GetGameInstance();

	UUIGameInstanceSubsystem* UISubSystem =
		GameInst->GetSubsystem<UUIGameInstanceSubsystem>();

	UISubSystem->RemoveWidget(mWidgetName);
}

void UBaseUserWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}
