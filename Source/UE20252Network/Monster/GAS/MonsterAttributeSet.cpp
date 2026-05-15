// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterAttributeSet.h"
#include "MonsterGAS.h"

void UMonsterAttributeSet::CallbackHP(AActor* Instigator)
{
	UAbilitySystemComponent* ASC = GetOwningAbilitySystemComponent();

	if (IsValid(ASC))
	{
		AMonsterGAS* Monster = Cast<AMonsterGAS>(ASC->GetAvatarActor());

		if (IsValid(Monster))
		{
			Monster->CallbackHP(Instigator);
		}
	}
}
