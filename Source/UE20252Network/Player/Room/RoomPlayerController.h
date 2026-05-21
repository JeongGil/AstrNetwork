// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "RoomPlayerController.generated.h"

class URoomWidget;
/**
 *
 */
UCLASS()
class UE20252NETWORK_API ARoomPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ARoomPlayerController();

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<URoomWidget> RoomWidget;

	FTimerHandle TimerHandle;

public:
	void RegisterGameState();

	UFUNCTION()
	void OnPlayerListChanged();

	UFUNCTION()
	void OnPlayerLogout(const FString& PlayerName);

protected:
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* aPawn) override;
	virtual void OnUnPossess() override;

public:
	[[nodiscard]] URoomWidget* GetRoomWidget() const
	{
		return RoomWidget;
	}
};
