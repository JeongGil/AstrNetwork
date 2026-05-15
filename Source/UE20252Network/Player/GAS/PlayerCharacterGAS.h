// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../GameInfo.h"
#include "PlayerAbilitySystemComponent.h"
#include "InputActionValue.h"
#include "../InventoryComponent.h"
#include "GameFramework/Character.h"
#include "PlayerCharacterGAS.generated.h"

UCLASS()
class UE20252NETWORK_API APlayerCharacterGAS : public ACharacter,
	public IGenericTeamAgentInterface,
	public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacterGAS();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USpringArmComponent> mSpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> mCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UWidgetComponent> mHeadUpWidget;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInventoryComponent> mInventoryCom;

	TObjectPtr<class UPlayerInfoWidget>	mInfoWidget;

	TObjectPtr<class UPlayerAnimInstance>	mAnimInst;

	FGenericTeamId		mTeamID;

	FName				mPlayerName;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UPlayerAbilitySystemComponent>	mASC;

	TObjectPtr<class UPlayerAttributeSet>	mAttributeSet;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> mEquipWeapon;

	TSubclassOf<UCameraShakeBase>	mDefaultShake;

public:
	virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const
	{
		return Cast<UAbilitySystemComponent>(mASC);
	}

	class UPlayerAttributeSet* GetAttributeSet()	const
	{
		return mAttributeSet;
	}

	class UPlayerInfoWidget* GetInfoWidget()	const
	{
		return mInfoWidget;
	}

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser);


private:
	void MoveKey(const FInputActionValue& Value);
	void RotationKey(const FInputActionValue& Value);
	void JumpKey(const FInputActionValue& Value);
	void AttackKey(const FInputActionValue& Value);
	void Skill1Key(const FInputActionValue& Value);
	void Skill2Key(const FInputActionValue& Value);
	void Skill2ReleasedKey(const FInputActionValue& Value);
	void Skill3Key(const FInputActionValue& Value);
	void ColorChangeKey(const FInputActionValue& Value);

protected:
	virtual void InputAttack();
	virtual void Skill1();
	virtual void Skill2();
	virtual void Skill3();

public:
	virtual void NormalAttack();
	virtual void Skill1Casting();

	void OnHPChange(const FOnAttributeChangeData& HP);
	void OnMPChange(const FOnAttributeChangeData& MP);
	void OnGoldChange(const FOnAttributeChangeData& Gold);

	void Acquisition(const FItemTableInfo* ItemInfo);

	void EquipWeapon(UStaticMesh* WeaponMesh);
	void UnEquipWeapon();


public:
	virtual void SetGenericTeamId(const FGenericTeamId& TeamID);
	virtual FGenericTeamId GetGenericTeamId() const;
	virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const;
};
