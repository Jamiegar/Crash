// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "GAS/CrashAttributeSet.h"
#include "CrashCharacter.generated.h"


class UCrashAttributeSet;
class UCrashGameplayAbility;

UCLASS(Abstract)
class CRASH_API ACrashCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACrashCharacter();
	void SetDefaultMesh();
	
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	virtual void PossessedBy(AController* NewController) override;
	virtual void InitializeAttributes();
	virtual void GiveDefaultAbilities();
	
	void ApplyEffectToCrashCharacter(TSubclassOf<UGameplayEffect> Effect) const;
	UCrashAttributeSet* GetCrashAttributeSet() const { return Attributes; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetCharacterSpeed() { return GetVelocity().Length(); }
protected:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Abilities")
	TSubclassOf<class UGameplayEffect> DefaultAttributeEffect;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Abilities")
	TArray<TSubclassOf<UCrashGameplayAbility>> DefaultAbilities;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Getter="GetAbilitySystemComponent", Category= "Abilities")
	UAbilitySystemComponent* AbilityComponent;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Attributes")
	int NumberOfJumps = 3;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Attributes")
	int Lives = 3;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Attributes")
	float PercentageLife = 0;
	
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode) override;
	virtual void OnStartedFalling();
	virtual void Landed(const FHitResult& Hit) override;
	virtual void SetUpDefaultMovementValues();
	
private:

	UPROPERTY()
	UCrashAttributeSet* Attributes;
	


	
	

};





