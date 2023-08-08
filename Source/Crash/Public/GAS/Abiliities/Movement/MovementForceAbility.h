// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/Abiliities/CrashGameplayAbility.h"
#include "MovementForceAbility.generated.h"

/**
 * 
 */
UCLASS()
class CRASH_API UMovementForceAbility : public UCrashGameplayAbility
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Movement Ability")
	float LaunchForce = 1500.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Movement Ability")
	FVector LaunchDirection = FVector(0, 0, 1);

	UPROPERTY(EditDefaultsOnly, Category="Movement Ability")
	UAnimMontage* MovementMontage;
	
	UMovementForceAbility();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
};
