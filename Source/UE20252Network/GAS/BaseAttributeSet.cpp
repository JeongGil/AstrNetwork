// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseAttributeSet.h"

void UBaseAttributeSet::PostGameplayEffectExecute(
	const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	AActor* Instigator = nullptr;

	const FGameplayEffectContextHandle& Context = Data.EffectSpec.GetContext();

	if (Context.IsValid())
	{
		Instigator = Context.GetOriginalInstigator();

		if (!Instigator)
			Instigator = Context.GetInstigator();
	}

	if (Data.EvaluatedData.Attribute == GetMPAttribute())
	{
		UE_LOG(UELOG, Warning, TEXT("MP : %.2f / %.2f"), GetMP(), GetMPMax());
	}

	else if (Data.EvaluatedData.Attribute == GetHPAttribute())
	{
		UE_LOG(UELOG, Warning, TEXT("HP : %.2f / %.2f"), GetHP(), GetHPMax());

		SetHP(FMath::Clamp(GetHP(), 0.f, GetHPMax()));

		CallbackHP(Instigator);
	}

	else if (Data.EvaluatedData.Attribute == GetAttackAttribute())
	{
		UE_LOG(UELOG, Warning, TEXT("Attack : %.2f"), GetAttack());
	}
}

void UBaseAttributeSet::CallbackHP(AActor* Instigator)
{
}
