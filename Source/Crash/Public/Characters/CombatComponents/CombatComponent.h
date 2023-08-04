// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "Components/ActorComponent.h"
#include "CombatComponent.generated.h"


struct FGameplayEventData;
class UCrashGameplayAbility;
class UAbilitySystemComponent;
class ACrashCharacter;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CRASH_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UCombatComponent();
	
	UFUNCTION(BlueprintCallable, Category="Crash Character Owner")
	ACrashCharacter* GetOwningCrashCharacter() const;

	UFUNCTION(BlueprintCallable, Category="Crash Character Owner")
	UAbilitySystemComponent* GetOwnersAbilitySystemComponent() const;

	UFUNCTION(BlueprintCallable, Category="Abilities|Combat")
	bool PerformDamageTrace(FName BoneNameLocation);
	
};


