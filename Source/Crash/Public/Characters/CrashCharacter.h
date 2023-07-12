// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "GAS/CrashAbilitySystemComponent.h"
#include "GAS/CrashAttributeSet.h"
#include "CrashCharacter.generated.h"


class UKnockbackComponent;
class UBasicCombatComponent;
class UCrashGameplayAbility;

UCLASS(Abstract)
class CRASH_API ACrashCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACrashCharacter();
	void SetDefaultMesh() const;
	void AddDefaultAbilities();
	
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override { return AbilityComponent; };
	UCrashAttributeSet* GetCrashAttributeSet() const;
	UBasicCombatComponent* GetBasicCombatComponent() const { return  BasicCombatComponent; }

	virtual void PossessedBy(AController* NewController) override;
	virtual void InitializeAttributes();
	virtual void GiveDefaultAbilities();
	
	void ApplyEffectToCrashCharacter(TSubclassOf<UGameplayEffect> Effect) const;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsKnockedBack = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bIsShielded = false;

protected:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Abilities")
	TSubclassOf<class UGameplayEffect> DefaultAttributeEffect;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Abilities")
	TArray<TSubclassOf<UCrashGameplayAbility>> DefaultAbilities;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category= "Abilities")
	UCrashAbilitySystemComponent* AbilityComponent;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Abilities")
	UBasicCombatComponent* BasicCombatComponent;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Knockback")
	UKnockbackComponent* KnockbackComponent; 
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetCharacterSpeed() { return GetVelocity().Length(); }
	
	
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode) override;
	virtual void OnStartedFalling();
	virtual void Landed(const FHitResult& Hit) override;
	virtual void SetUpDefaultMovementValues();

private:
	UPROPERTY(EditDefaultsOnly)
	UCrashAttributeSet* CrashAttributes;
	
	
};





