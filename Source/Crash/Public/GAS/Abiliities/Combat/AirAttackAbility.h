// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/Abiliities/Combat/AttackAbility.h"
#include "AirAttackAbility.generated.h"

/**
 * 
 */
UCLASS()
class CRASH_API UAirAttackAbility : public UAttackAbility
{
	GENERATED_BODY()

public:
	UAirAttackAbility();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData) override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="AttackMontage")
	UAnimMontage* AttackMontage;

	UFUNCTION()
	void OnMontageFinished(UAnimMontage* AnimMontage, bool bInterrupted);

	virtual void OnGameplayReceivedDamageEvent(FGameplayEventData Payload) override;
};
