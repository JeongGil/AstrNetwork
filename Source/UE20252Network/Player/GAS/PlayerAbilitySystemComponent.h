// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../GameInfo.h"
#include "AbilitySystemComponent.h"
#include "PlayerAbilitySystemComponent.generated.h"

/**
 * 
 */
UCLASS()
class UE20252NETWORK_API UPlayerAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
	
protected:
	float	mSkillMana = 0.f;

public:
	void SetSkillMana(float Mana)
	{
		mSkillMana = Mana;
	}

	float GetSkillMana()	const
	{
		return mSkillMana;
	}
};
