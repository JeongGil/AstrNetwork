// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../GameInfo.h"
#include "Blueprint/DragDropOperation.h"
#include "IconDragDropOperation.generated.h"

/**
 * 
 */
UCLASS()
class UE20252NETWORK_API UIconDragDropOperation : public UDragDropOperation
{
	GENERATED_BODY()
	
public:
	UPROPERTY()
	class UIconWidget* mDragIcon = nullptr;

	UPROPERTY()
	class UGameSlotWidget* mStartSlot = nullptr;
};
