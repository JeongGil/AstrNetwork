// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../GAS/BaseAttributeSet.h"
#include "PlayerAttributeSet.generated.h"

/**
 * 
 */
UCLASS()
class UE20252NETWORK_API UPlayerAttributeSet : public UBaseAttributeSet
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attribute", meta = (AllowPrivateAccess = "true"))
	FGameplayAttributeData	Job;


public:
	virtual void CallbackHP(AActor* Instigator);

public:
	ATTRIBUTE_ACCESSORS(UPlayerAttributeSet, Job)
};
