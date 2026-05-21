// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UE20252Network/UI/BaseUserWidget.h"
#include "RoomWidget.generated.h"

class ARoomPlayerState;
class UListView;
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

public:
	void RefreshPlayerList(TArray<ARoomPlayerState*> Array);
	void PlayerLogout(TArray<ARoomPlayerState*> Array, const FString& LogoutPlayerName);

protected:
	virtual void NativeOnInitialized() override;
};
