// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameplayTagContainer.h"
#include "GameplayEffectTypes.h"
#include "AbilitySystemInterface.h"
#include "CrashCharacter.generated.h"

UCLASS()
class CRASH_API ACrashCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACrashCharacter();
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void PossessedBy(AController* NewController) override;
	virtual void InitializeAttributes();
	virtual void GiveDefaultAbilities();

protected:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Abilities")
	TSubclassOf<class UGameplayEffect> DefaultAttributeEffect;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Abilities")
	TArray<TSubclassOf<UGameplayAbility>> DefaultAbilities;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


private:
	UPROPERTY(VisibleAnywhere, Category= "Abilities")
	UAbilitySystemComponent* AbilityComponent;

	UPROPERTY()
	class UCrashAttributeSet* Attributes;


	
	

};
