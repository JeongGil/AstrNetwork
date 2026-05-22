// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_PlayerAttack.h"

#include "GAS/PlayerCharacterGAS.h"

void UAnimNotify_PlayerAttack::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                      const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	UE_LOG(UELOG, Warning, TEXT("AnimNotify_PlayerAttack: Notify() triggered. Animation: [%s]"), 
		Animation ? *Animation->GetName() : TEXT("None"));

	if (!IsValid(MeshComp))
	{
		UE_LOG(UELOG, Warning, TEXT("MeshComp is invalid in UAnimNotify_PlayerAttack::Notify"));
		return;
	}

	auto* PlayerCharacter = MeshComp->GetOwner<APlayerCharacterGAS>();
	if (!IsValid(PlayerCharacter))
	{
		UE_LOG(UELOG, Warning, TEXT("PlayerCharacter is invalid in UAnimNotify_PlayerAttack::Notify. Owner: [%s]"), 
			MeshComp->GetOwner() ? *MeshComp->GetOwner()->GetName() : TEXT("None"));
		return;
	}

	bool bIsLocallyControlled = PlayerCharacter->IsLocallyControlled();
	FString ActorName = PlayerCharacter->GetName();
	
	UE_LOG(UELOG, Warning, TEXT("AnimNotify_PlayerAttack: Actor [%s], IsLocallyControlled [%s]"), 
		*ActorName, bIsLocallyControlled ? TEXT("True") : TEXT("False"));

	if (bIsLocallyControlled)
	{
		UE_LOG(UELOG, Warning, TEXT("Calling SvrNormalAttack for Actor [%s]"), *ActorName);
		PlayerCharacter->SvrNormalAttack();
	}
	else
	{
		UE_LOG(UELOG, Warning, TEXT("PlayerCharacter is not locally controlled in UAnimNotify_PlayerAttack::Notify"));
	}
}
