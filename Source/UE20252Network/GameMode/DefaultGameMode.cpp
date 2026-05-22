// Fill out your copyright notice in the Description page of Project Settings.


#include "DefaultGameMode.h"
#include "../Player/MainPlayerController.h"
#include "../Player/MainPlayerState.h"
#include "../Player/GAS/ShinbiGAS.h"
#include "../Player/GAS/WraithGAS.h"

ADefaultGameMode::ADefaultGameMode()
{
	PrimaryActorTick.bCanEverTick = true;

	// StaticClass 함수는 GENERATED_BODY() 매크로에서 자동으로 생성해주는 static 멤버함수이다.
	// 이 함수는 클래스의 UClass 정보를 반환한다.
	//DefaultPawnClass = AShinbi::StaticClass();
	//DefaultPawnClass = AWraith::StaticClass();
	// DefaultPawnClass = AShinbiGAS::StaticClass();
	DefaultPawnClass = AWraithGAS::StaticClass();

	PlayerStateClass = AMainPlayerState::StaticClass();

	PlayerControllerClass = AMainPlayerController::StaticClass();
}

APlayerController* ADefaultGameMode::Login(UPlayer* NewPlayer, ENetRole InRemoteRole,
	const FString& Portal, const FString& Options, const FUniqueNetIdRepl& UniqueId,
	FString& ErrorMessage)
{
	APlayerController* Result = Super::Login(NewPlayer, InRemoteRole, Portal,
		Options, UniqueId, ErrorMessage);

	int32	Job = 0;
	FParse::Value(*Options, TEXT("Job="), Job);

	switch ((EPlayerJob)Job)
	{
	case EPlayerJob::Knight:
		DefaultPawnClass = AShinbiGAS::StaticClass();
		break;
	case EPlayerJob::Archer:
		DefaultPawnClass = AWraithGAS::StaticClass();
		break;
	case EPlayerJob::Magicion:
		break;
	}

	FString	PlayerName;
	FParse::Value(*Options, TEXT("PlayerName="), PlayerName);

	AMainPlayerController* MainController = Cast<AMainPlayerController>(Result);

	MainController->SetPlayerName(PlayerName);

	return Result;
}
