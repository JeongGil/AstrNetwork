// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "GameFramework/PlayerController.h"
#include "MainPlayerController.generated.h"


UCLASS()
class UE20252NETWORK_API AMainPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AMainPlayerController();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UMainWidget>	mMainWidgetClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UMainWidget>	mMainWidget;

	FString			mPlayerName;

public:
	class UMainWidget* GetMainWidget()	const
	{
		return mMainWidget;
	}

	FString GetPlayerName()	const
	{
		return mPlayerName;
	}

	void SetPlayerName(const FString& PlayerName)
	{
		mPlayerName = PlayerName;
	}

protected:
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* aPawn) override;
	virtual void OnUnPossess() override;

public:
	virtual void Tick(float DeltaTime) override;
};
