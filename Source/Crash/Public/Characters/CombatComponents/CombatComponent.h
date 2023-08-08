// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "Components/ActorComponent.h"
#include "CombatComponent.generated.h"


class AHitBox;
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

	UFUNCTION(BlueprintCallable, Category="Combat")
	bool PerformDamageTrace(FName BoneNameLocation);

	UFUNCTION(BlueprintCallable, Category="Combat")
	AHitBox* SpawnHitBox(FVector Location, FRotator Rotation);

private:
	UFUNCTION()
	void OnHitBoxOverlapWithActor(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
};


