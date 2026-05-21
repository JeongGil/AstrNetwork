// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UE20252Network/UI/BaseUserWidget.h"
#include "RoomWidget.generated.h"

class UTextBlock;
class UButton;
class ARoomPlayerState;
class UListView;

UENUM()
enum class ERoomButtonType : uint8
{
	Start,
	Ready,
};

/**
 *
 */
UCLASS()
class UE20252NETWORK_API URoomWidget : public UBaseUserWidget
{
	GENERATED_BODY()

public:
	URoomWidget(const FObjectInitializer& ObjectInitializer);

protected:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UListView> mPlayerList;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> mButtonText;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> mStartButton;

	ERoomButtonType ButtonType = ERoomButtonType::Start;
	bool bReady = false;

public:
	void RefreshPlayerList(TArray<ARoomPlayerState*> Array);
	void PlayerLogout(TArray<ARoomPlayerState*> Array, const FString& LogoutPlayerName);

	void SetButtonText(const FString& Text);
	void SetButtonEnable(bool bEnable);
	void EnableReadyButton();

	UFUNCTION()
	void ButtonClick();

protected:
	virtual void NativeOnInitialized() override;
};
