// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_PlayerAttack.h"

#include "GAS/PlayerCharacterGAS.h"

void UAnimNotify_PlayerAttack::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                      const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (!IsValid(MeshComp))
	{
		UE_LOG(UELOG, Warning, TEXT("MeshComp is invalid in UAnimNotify_PlayerAttack::Notify"));
		return;
	}

	auto* PlayerCharacter = MeshComp->GetOwner<APlayerCharacterGAS>();
	if (!IsValid(PlayerCharacter))
	{
		UE_LOG(UELOG, Warning, TEXT("PlayerCharacter is invalid in UAnimNotify_PlayerAttack::Notify"))
		return;
	}

	if (PlayerCharacter->IsLocallyControlled())
	{
		PlayerCharacter->SvrNormalAttack();
	}
}
