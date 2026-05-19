// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "BaseUserWidget.generated.h"

/**
 *
 */
UCLASS()
class UE20252NETWORK_API UBaseUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UBaseUserWidget(const FObjectInitializer& ObjectInitializer);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString mWidgetName;

	AActor* mOwnerActor = nullptr;

	TMap<FString, TObjectPtr<UWidgetAnimation>>	mAnimMap;

public:
	void SetOwnerActor(AActor* Actor)
	{
		mOwnerActor = Actor;
	}

	void PlayWidgetAnimation(const FString& Name, float PlayStartTime = 0.f,
		float PlaySpeed = 1.f, bool Forward = true,
		bool RestoreState = false, int32 LoopCount = 1);

protected:
	virtual void NativeOnInitialized();
	virtual void NativePreConstruct();
	virtual void NativeConstruct();
	virtual void NativeDestruct();
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);
};
