// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_PlayerAttack.h"

#include "GAS/PlayerCharacterGAS.h"

void UAnimNotify_PlayerAttack::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                      const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	auto* PlayerCharacter = MeshComp->GetOwner<APlayerCharacterGAS>();
	if (IsValid(PlayerCharacter))
	{
		PlayerCharacter->SvrNormalAttack();
	}
}
