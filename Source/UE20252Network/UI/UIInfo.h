#pragma once

#include "EngineMinimal.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "Components/EditableTextBox.h"
#include "Components/Border.h"
#include "Components/ScrollBox.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/ScrollBoxSlot.h"
#include "Components/ComboBoxString.h"
#include "Components/Slider.h"
#include "Components/ListView.h"
#include "Components/TileView.h"
#include "Components/TreeView.h"

#include "Animation/WidgetAnimation.h"
#include "Blueprint/WidgetBlueprintGeneratedClass.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Blueprint/SlateBlueprintLibrary.h"
#include "Blueprint/WidgetLayoutLibrary.h"

UENUM(BlueprintType)
enum class ESlotWidgetType : uint8
{
	Inventory,
	Quick
};
