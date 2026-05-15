// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "GameFramework/PlayerState.h"
#include "MainPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class UE20252NETWORK_API AMainPlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:
	AMainPlayerState();

protected:
	TObjectPtr<UDataTable>	mPlayerInfoTable;
	FName					mTableRowName;

protected:
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	FString		mName;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	EPlayerJob	mJob;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	int32		mLevel = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	int32		mExp = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float		mAttack = 0.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float		mDefense = 0.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float		mHP = 0.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float		mHPMax = 0.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float		mMP = 0.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float		mMPMax = 0.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float		mWalkSpeed = 0.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float		mRunSpeed = 0.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float		mAttackDistance = 0.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	int32		mGold = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TMap<EAbilityType, float>	mAbilityMap;

public:
	const FString& GetPlayerName()	const
	{
		return mName;
	}

	EPlayerJob GetPlayerJob()	const
	{
		return mJob;
	}

	int32 GetLevel()	const
	{
		return mLevel;
	}

	int32 GetExp()	const
	{
		return mExp;
	}

	float GetAttack()	const
	{
		return mAttack;
	}

	float GetDefense()	const
	{
		return mDefense;
	}

	float GetHP()	const
	{
		return mHP;
	}

	float GetHPMax()	const
	{
		return mHPMax;
	}

	float GetMP()	const
	{
		return mMP;
	}

	float GetMPMax()	const
	{
		return mMPMax;
	}

	float GetWalkSpeed()	const
	{
		return mWalkSpeed;
	}

	float GetRunSpeed()	const
	{
		return mRunSpeed;
	}

	float GetAttackDistance()	const
	{
		return mAttackDistance;
	}

	int32 GetGold()	const
	{
		return mGold;
	}

	const float GetAbility(EAbilityType Type)
	{
		if (!mAbilityMap.Contains(Type))
			return -1.f;

		return *mAbilityMap.Find(Type);
	}

public:
	void SetName(const FString& Name)
	{
		mName = Name;
	}

	void SetLevel(int32 Level)
	{
		mLevel = Level;
	}

	void AddLevel(int32 Level = 1)
	{
		mLevel += Level;
	}

	void SetExp(int32 Exp)
	{
		mExp = Exp;
	}

	void AddExp(int32 Exp)
	{
		mExp += Exp;
	}

	void SetAttack(float Attack)
	{
		mAttack = Attack;
	}

	void AddAttack(float Attack)
	{
		mAttack += Attack;
	}

	void SetDefense(float Defense)
	{
		mDefense = Defense;
	}

	void AddDefense(float Defense)
	{
		mDefense += Defense;
	}

	bool SetHP(float HP)
	{
		mHP = HP;

		if (mHP > mHPMax)
			mHP = mHPMax;

		else if (mHP < 0)
		{
			mHP = 0;
			return true;
		}

		return false;
	}

	bool AddHP(float HP)
	{
		mHP += HP;

		if (mHP > mHPMax)
			mHP = mHPMax;

		else if (mHP < 0)
		{
			mHP = 0;
			return true;
		}

		return false;
	}

	void SetHPMax(float HP)
	{
		mHPMax = HP;
	}

	void AddHPMax(float HP)
	{
		mHPMax += HP;
	}

	void SetMP(float MP)
	{
		mMP = MP;

		if (mMP > mMPMax)
			mMP = mMPMax;

		else if (mMP < 0)
			mMP = 0;
	}

	void AddMP(float MP)
	{
		mMP += MP;

		if (mMP > mMPMax)
			mMP = mMPMax;

		else if (mMP < 0)
			mMP = 0;
	}

	void SetMPMax(float MP)
	{
		mMPMax = MP;
	}

	void AddMPMax(float MP)
	{
		mMPMax += MP;
	}

	void SetWalkSpeed(float Speed)
	{
		mWalkSpeed = Speed;
	}

	void AddWalkSpeed(float Speed)
	{
		mWalkSpeed += Speed;
	}

	void SetRunSpeed(float Speed)
	{
		mRunSpeed = Speed;
	}

	void AddRunSpeed(float Speed)
	{
		mRunSpeed += Speed;
	}

	void SetAttackDistance(float AttackDistance)
	{
		mAttackDistance = AttackDistance;
	}

	void AddAttackDistance(float AttackDistance)
	{
		mAttackDistance += AttackDistance;
	}

	void SetGold(uint32 Gold)
	{
		mGold = Gold;

		if (mGold < 0)
			mGold = 0;
	}

	void AddGold(uint32 Gold)
	{
		mGold += Gold;

		if (mGold < 0)
			mGold = 0;
	}

	void SetAbility(EAbilityType Type, float Ability)
	{
		if (!mAbilityMap.Contains(Type))
			return;

		mAbilityMap[Type] = Ability;
	}

	void AddAbility(EAbilityType Type, float Ability)
	{
		if (!mAbilityMap.Contains(Type))
			return;

		mAbilityMap[Type] += Ability;
	}

protected:
	virtual void BeginPlay() override;

public:
	void LoadPlayerInfo(const FName& PlayerName);

	UFUNCTION()
	void PlayerInfoLoadComplete(FPrimaryAssetId LoadId);
};
