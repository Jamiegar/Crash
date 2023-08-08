// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/Abiliities/Combat/AttackAbility.h"
#include "MovementAttackAbility.generated.h"

/**
 * 
 */
UCLASS()
class CRASH_API UMovementAttackAbility : public UAttackAbility
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Movement")
	float LaunchForce = 1200;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Movement")
	FVector LaunchDirection = FVector(0, 0, 1);

	UPROPERTY(EditDefaultsOnly, Category="Movement")
	UAnimMontage* AttackMontage;
	

	
	UMovementAttackAbility();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

protected:
	virtual void OnMovementMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	virtual void OnGameplayReceivedDamageEvent(FGameplayEventData Payload) override;
};
