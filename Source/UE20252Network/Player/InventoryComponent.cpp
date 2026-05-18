// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"
#include "../Etc/ItemObject.h"
#include "../GameMode/AssetGameInstanceSubsystem.h"
#include "../GameMode/UIGameInstanceSubsystem.h"
#include "../UI/Main/InventoryWidget.h"
#include "Engine/ActorChannel.h"
#include "GAS/PlayerCharacterGAS.h"
#include "GAS/Effect/GameplayEffect_HPPotion.h"
#include "GAS/Effect/GameplayEffect_ItemAttack.h"
#include "Net/UnrealNetwork.h"

UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	bWantsInitializeComponent = true;

	SetIsReplicated(true);
}

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	if (!GetOwner() || !GetOwner()->HasAuthority())
	{
		return;
	}

	UGameInstance* GameInst = GetWorld()->GetGameInstance();

	// 테스트에 사용할 아이템 생성.
	auto* AssetSubSystem = GameInst->GetSubsystem<UAssetGameInstanceSubsystem>();

	if (AssetSubSystem->GetItemInfoLoadComplete())
	{
		ItemInfoLoadComplete();
	}
	else
	{
		AssetSubSystem->mItemInfoLoadDelegate.AddUObject(this, &UInventoryComponent::ItemInfoLoadComplete);
	}
}

void UInventoryComponent::InitializeComponent()
{
	Super::InitializeComponent();

	mItemList.Init(nullptr, mInventoryMaxCount);
}

void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                        FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

bool UInventoryComponent::ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags)
{
	bool Result = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);

	for (auto& Item : mItemList)
	{
		Result |= Channel->ReplicateSubobject(Item, *Bunch, *RepFlags);
	}

	return Result;
}

void UInventoryComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UInventoryComponent, mItemList);
	DOREPLIFETIME(UInventoryComponent, mItemCount);
}

void UInventoryComponent::AddItem(const FItemTableInfo* Info)
{
	if (mItemCount == mInventoryMaxCount)
	{
		return;
	}

	// 이 아이템이 수량추가가 가능한 아이템인지 체크한다.
	if (Info->WidgetLayeredType == EItemWidgetLayerType::Layered)
	{
		bool Layered = false;

		for (int32 i = 0; i < mInventoryMaxCount; ++i)
		{
			if (IsValid(mItemList[i]))
			{
				if (mItemList[i]->GetItemName() == Info->ItemName)
				{
					Layered = true;
					mItemList[i]->AddCount();

					// 위젯에 수량 변화를 알려준다.
					if (mItemCountChange.IsBound())
					{
						mItemCountChange.Broadcast(i, mItemList[i]->GetItemCount());
					}

					break;
				}
			}
		}

		if (!Layered)
		{
			UItemObject* Item = NewObject<UItemObject>();

			Item->SetItemInfo(Info);

			for (int32 i = 0; i < mInventoryMaxCount; ++i)
			{
				if (!IsValid(mItemList[i]))
				{
					mItemList[i] = Item;

					SetItemCli(Item, i);

					GEngine->AddOnScreenDebugMessage(-1, 1000.f, FColor::Red, TEXT("AddItem"));

					++mItemCount;

					// 델리게이트를 만들어서 인벤토리에 변화가 생긴다면 호출할 함수를 등록하여
					// 여기에서 그 함수들을 호출해준다.
					if (mItemChange.IsBound())
					{
						mItemChange.Broadcast(Item, i);
					}

					break;
				}
			}
		}
	}
	else
	{
		UItemObject* Item = NewObject<UItemObject>(GetOwner());

		Item->SetItemInfo(Info);

		for (int32 i = 0; i < mInventoryMaxCount; ++i)
		{
			if (!IsValid(mItemList[i]))
			{
				mItemList[i] = Item;

				SetItemCli(Item, i);

				++mItemCount;

				// 델리게이트를 만들어서 인벤토리에 변화가 생긴다면 호출할 함수를 등록하여
				// 여기에서 그 함수들을 호출해준다.
				if (mItemChange.IsBound())
				{
					mItemChange.Broadcast(Item, i);
				}

				break;
			}
		}
	}
}

void UInventoryComponent::ChangeGold(int32 Gold)
{
	if (mGoldChange.IsBound())
	{
		mGoldChange.Broadcast(Gold);
	}
}

void UInventoryComponent::ItemInfoLoadComplete()
{
	GEngine->AddOnScreenDebugMessage(-1, 1000.f, FColor::Green, TEXT("ItemInfoLoadComplete"));

	const UGameInstance* GameInst = GetWorld()->GetGameInstance();

	// 테스트에 사용할 아이템 생성.
	const auto* AssetSubSystem = GameInst->GetSubsystem<UAssetGameInstanceSubsystem>();

	TArray<FName> ItemNames;
	ItemNames.Add(TEXT("sword_001"));
	ItemNames.Add(TEXT("sword_002"));
	ItemNames.Add(TEXT("bow_001"));
	ItemNames.Add(TEXT("armor_001"));
	ItemNames.Add(TEXT("potion_001"));
	ItemNames.Add(TEXT("potion_001"));

	for (int32 i = 0; i < 6; ++i)
	{
		const FItemTableInfo* Info = AssetSubSystem->FindItemInfo(ItemNames[i]);

		if (!Info)
		{
			continue;
		}

		AddItem(Info);
	}

	auto* UISubSystem = GameInst->GetSubsystem<UUIGameInstanceSubsystem>();

	auto* InventoryWidget = UISubSystem->FindWidget<UInventoryWidget>(TEXT("Inventory"));

	if (IsValid(InventoryWidget))
	{
		InventoryWidget->InitInventory(this);
	}
}

void UInventoryComponent::UseItem(int32 Index)
{
	if (Index < 0 || Index >= mInventoryMaxCount)
	{
		return;
	}
	else if (!IsValid(mItemList[Index]))
	{
		return;
	}

	switch (mItemList[Index]->GetItemType())
	{
	case EItemType::Sword:
	case EItemType::Bow:
	case EItemType::Staff:
	case EItemType::Gun:
		UseWeapon(Index);
		break;
	case EItemType::Armor:
	case EItemType::Helmet:
	case EItemType::Glove:
	case EItemType::Shoes:
		UseArmor(Index);
		break;
	case EItemType::Potion:
		UsePotion(Index);
		break;
	case EItemType::Scroll:
		break;
	}
}

void UInventoryComponent::SwapItem(int32 StartIndex, int32 ChangeIndex)
{
	mItemList.Swap(StartIndex, ChangeIndex);

	if (mItemChange.IsBound())
	{
		mItemChange.Broadcast(mItemList[StartIndex], StartIndex);
		mItemChange.Broadcast(mItemList[ChangeIndex], ChangeIndex);
	}
}

void UInventoryComponent::RemoveItem(int32 Index)
{
}

bool UInventoryComponent::CheckItem(int32 Index)
{
	return true;
}

void UInventoryComponent::UsePotion(int32 Index)
{
	APlayerCharacterGAS* OwnerCharacter = Cast<APlayerCharacterGAS>(GetOwner());

	if (!OwnerCharacter)
	{
		return;
	}

	UAbilitySystemComponent* ASC = OwnerCharacter->GetAbilitySystemComponent();
	if (!ASC)
	{
		return;
	}

	const TArray<FItemOption>& Options = mItemList[Index]->GetItemOptions();
	for (const FItemOption& Option : Options)
	{
		bool Loop = true;

		switch (Option.Type)
		{
		case EItemOptionType::Attack:
			break;
		case EItemOptionType::Defense:
			break;
		case EItemOptionType::HPMax:
			break;
		case EItemOptionType::MPMax:
			break;
		case EItemOptionType::HPRecoveryPoint:
			{
				// 체력 회복 GameplayEffect 동작.
				FGameplayEffectContextHandle ContextHandle = ASC->MakeEffectContext();

				ContextHandle.AddSourceObject(OwnerCharacter);

				FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(
					UGameplayEffect_HPPotion::StaticClass(), 1.f, ContextHandle);

				if (!SpecHandle.IsValid())
				{
					continue;
				}

				SpecHandle.Data->SetSetByCallerMagnitude(
					FGameplayTag::RequestGameplayTag(TEXT("Effect.Item.HPPotion")),
					Option.Option);

				ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data);

				mItemList[Index]->AddCount(-1);

				if (mItemCountChange.IsBound())
				{
					mItemCountChange.Broadcast(Index, mItemList[Index]->GetItemCount());
				}

				if (mItemList[Index]->GetItemCount() == 0)
				{
					mItemList[Index] = nullptr;
					Loop = false;

					if (mItemChange.IsBound())
					{
						mItemChange.Broadcast(nullptr, Index);
					}
				}
			}
			break;
		case EItemOptionType::HPRecoveryRate:
			break;
		case EItemOptionType::MPRecoveryPoint:
			break;
		case EItemOptionType::MPRecoveryRate:
			break;
		case EItemOptionType::MoveSpeed:
			break;
		case EItemOptionType::AttackSpeed:
			break;
		case EItemOptionType::AttackDistance:
			break;
		case EItemOptionType::CriticalRatio:
			break;
		case EItemOptionType::CriticalDamage:
			break;
		}

		if (!Loop)
		{
			break;
		}
	}
}

void UInventoryComponent::UseWeapon(int32 Index)
{
	// 기존에 장착하고 있던 아이템을 다시 장착하려고 하는 경우
	if (mEquipWeaponIndex == Index)
	{
		return;
	}

	auto* OwnerCharacter = Cast<APlayerCharacterGAS>(GetOwner());
	if (!OwnerCharacter)
	{
		return;
	}

	// 기존에 장착하고 있던 무기가 있을 경우
	if (mEquipWeaponIndex != -1)
	{
		RemoveItemAttack(mEquipWeaponIndex);

		// 기존에 있던 장착 표시를 해제한다.
		if (mItemEquipChange.IsBound())
		{
			mItemEquipChange.Broadcast(mEquipWeaponIndex, false);
		}
	}

	ApplyItemAttack(Index);

	// 아이템 Mesh를 장착시킨다.
	OwnerCharacter->EquipWeapon(mItemList[Index]->GetItemMesh());

	if (mItemEquipChange.IsBound())
	{
		mItemEquipChange.Broadcast(Index, true);
	}

	mEquipWeaponIndex = Index;
}

void UInventoryComponent::UseArmor(int32 Index)
{
}

float UInventoryComponent::GetItemAttack(int32 Index)
{
	float Attack = 0.f;

	const TArray<FItemOption>& Options = mItemList[Index]->GetItemOptions();
	for (const auto& [Type, Option] : Options)
	{
		if (Type == EItemOptionType::Attack)
		{
			Attack += Option;
		}
	}

	return Attack;
}

void UInventoryComponent::ApplyItemAttack(int32 Index)
{
	const auto* OwnerCharacter = Cast<APlayerCharacterGAS>(GetOwner());
	if (!OwnerCharacter)
	{
		return;
	}

	UAbilitySystemComponent* ASC = OwnerCharacter->GetAbilitySystemComponent();
	if (!ASC)
	{
		return;
	}

	// 체력 회복 GameplayEffect 동작.
	FGameplayEffectContextHandle ContextHandle = ASC->MakeEffectContext();

	ContextHandle.AddSourceObject(OwnerCharacter);

	FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(UGameplayEffect_ItemAttack::StaticClass(), 1.f,
	                                                             ContextHandle);

	if (!SpecHandle.IsValid())
	{
		return;
	}

	SpecHandle.Data->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(TEXT("Effect.Item.Weapon")),
	                                         GetItemAttack(Index));

	mWeaponHandle = ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data);
}

void UInventoryComponent::RemoveItemAttack(int32 Index)
{
	const auto* OwnerCharacter = Cast<APlayerCharacterGAS>(GetOwner());
	if (!OwnerCharacter)
	{
		return;
	}

	UAbilitySystemComponent* ASC = OwnerCharacter->GetAbilitySystemComponent();
	if (!ASC)
	{
		return;
	}

	// Infinite로 했을 경우
	ASC->RemoveActiveGameplayEffect(mWeaponHandle);

	return;

	// 체력 회복 GameplayEffect 동작.
	FGameplayEffectContextHandle ContextHandle = ASC->MakeEffectContext();

	ContextHandle.AddSourceObject(OwnerCharacter);

	FGameplayEffectSpecHandle SpecHandle =
		ASC->MakeOutgoingSpec(UGameplayEffect_ItemAttack::StaticClass(), 1.f,
		                      ContextHandle);

	if (!SpecHandle.IsValid())
		return;

	SpecHandle.Data->SetSetByCallerMagnitude(
		FGameplayTag::RequestGameplayTag(TEXT("Effect.Item.Weapon")),
		-GetItemAttack(Index));

	ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data);
}

void UInventoryComponent::SetItemCli_Implementation(UItemObject* Item, int32 Index)
{
	mItemList[Index] = Item;

	GEngine->AddOnScreenDebugMessage(-1, 1000.f, FColor::Red, TEXT("AddItem"));

	if (mItemChange.IsBound())
	{
		mItemChange.Broadcast(Item, Index);
	}
}
