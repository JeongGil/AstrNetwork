// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayerState.h"
#include "PlayerInfoTableAsset.h"
#include "GAS/PlayerCharacterGAS.h"
#include "GAS/PlayerAttributeSet.h"
#include "MainPlayerController.h"
#include "../UI/Main/MainWidget.h"
#include "../UI/Main/PlayerInfoWidget.h"

AMainPlayerState::AMainPlayerState()
{
}

void AMainPlayerState::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(UELOG, Warning, TEXT("PlayerState BeginPlay"));


}

void AMainPlayerState::LoadPlayerInfo(const FName& PlayerName)
{
	mTableRowName = PlayerName;

	UAssetManager& AssetMgr = UAssetManager::Get();

	FPrimaryAssetId	PlayerAssetId(TEXT("PlayerInfoTableAsset"), TEXT("PDA_PlayerInfo"));

	// 비동기 로딩을 진행해준다. 로딩 명령을 내려놓고 로딩이 끝나면 지정된 함수가
	// 호출된다.
	AssetMgr.LoadPrimaryAsset(PlayerAssetId, TArray<FName>{},
		FStreamableDelegate::CreateUObject(this, &AMainPlayerState::PlayerInfoLoadComplete, PlayerAssetId));
}

void AMainPlayerState::PlayerInfoLoadComplete(FPrimaryAssetId LoadId)
{
	UAssetManager& AssetMgr = UAssetManager::Get();

	// 로딩된 오브젝트를 얻어온다.
	TObjectPtr<UObject>	LoadObject = AssetMgr.GetPrimaryAssetObject(LoadId);

	TObjectPtr<UPlayerInfoTableAsset>	DataAsset =
		Cast<UPlayerInfoTableAsset>(LoadObject);

	if (!DataAsset)
	{
		UE_LOG(UELOG, Warning, TEXT("PlayerInfo Load Failed"));
		return;
	}

	mPlayerInfoTable = DataAsset->mTable.LoadSynchronous();

	if (!IsValid(mPlayerInfoTable))
	{
		UE_LOG(UELOG, Warning, TEXT("PlayerInfo DataTable Load Failed"));
		return;
	}

	UE_LOG(UELOG, Warning, TEXT("PlayerInfo Load Complete"));

	const FPlayerInfo* Info = mPlayerInfoTable->FindRow<FPlayerInfo>(
		mTableRowName, TEXT("PlayerInfo"));

	mName = Info->PlayerName;

	AMainPlayerController* PlayerCtrl = Cast<AMainPlayerController>(GetOwner());

	if (IsValid(PlayerCtrl))
	{
		UMainWidget* MainWidget = PlayerCtrl->GetMainWidget();

		if (IsValid(MainWidget))
			MainWidget->GetPlayerHUDWidget()->SetPlayerName(mName);
	}

	mJob = Info->Job;
	mLevel = Info->Level;
	mExp = Info->Exp;
	mAttack = Info->Attack;
	mDefense = Info->Defense;
	mHPMax = Info->HPMax;
	mHP = mHPMax;
	mMPMax = Info->MPMax;
	mMP = mMPMax;


	mWalkSpeed = Info->WalkSpeed;
	mRunSpeed = Info->RunSpeed;
	mAttackDistance = Info->AttackDistance;
	mGold = Info->Gold;

	mAbilityMap = Info->AbilityMap;

	if (PlayerCtrl)
	{
		ACharacter* Character = PlayerCtrl->GetPawn<ACharacter>();

		Character->GetCharacterMovement()->MaxWalkSpeed = mRunSpeed;

		APlayerCharacterGAS* GAS = Cast<APlayerCharacterGAS>(Character);

		if (IsValid(GAS))
		{
			UPlayerInfoWidget* InfoWidget = GAS->GetInfoWidget();

			if (IsValid(InfoWidget))
			{
				InfoWidget->SetPlayerName(mName);
			}

			UPlayerAttributeSet* Attr = GAS->GetAttributeSet();

			if (IsValid(Attr))
			{
				UE_LOG(UELOG, Warning, TEXT("Attribute Set Complete"));
				Attr->SetAttack(mAttack);
				Attr->SetDefense(mDefense);
				Attr->SetHP(mHP);
				Attr->SetHPMax(mHPMax);
				Attr->SetMP(mMP);
				Attr->SetMPMax(mMPMax);
				Attr->SetWalkSpeed(mWalkSpeed);
				Attr->SetRunSpeed(mRunSpeed);
				Attr->SetAttackDistance(mAttackDistance);
				Attr->SetGold(mGold);
				Attr->SetJob((float)mJob);
			}
		}
	}
}
