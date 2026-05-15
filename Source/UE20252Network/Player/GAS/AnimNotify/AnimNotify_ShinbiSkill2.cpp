// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_ShinbiSkill2.h"

void UAnimNotify_ShinbiSkill2::Notify(USkeletalMeshComponent* MeshComp,
	UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (!mEventTag.IsValid())
		return;

	if (!MeshComp)
		return;

	AActor* OwnerActor = MeshComp->GetOwner();

	if (!IsValid(OwnerActor))
		return;

	FGameplayEventData	EventData;
	EventData.EventTag = mEventTag;
	EventData.Instigator = OwnerActor;

	// OwnerActor 에게 GameplayEvent를 보낸다.
	// GAS에서는 이 이벤트를 Ability가 받을 수 있다.
	// Actor에게 Tag의 이벤트 신호를 보내면 Actor가 가지고 있는 AbilitySystemComponent가
	// 이벤트를 받고 실행 중인 Ability안의 WaitGameplayEvent가 반응하게 한다.
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(OwnerActor, mEventTag,
		EventData);
}

