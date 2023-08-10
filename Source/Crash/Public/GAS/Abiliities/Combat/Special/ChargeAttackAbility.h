// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/Abiliities/Combat/AttackAbility.h"
#include "ChargeAttackAbility.generated.h"

class UAbilityTask_WaitInputRelease;

/**
 * 
 */
UCLASS()
class CRASH_API UChargeAttackAbility : public UAttackAbility
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Charge")
	UAnimMontage* AttackMontage;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Charge")
	float DamageMultiplier = 5.0f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input Duration")
	float MaxInputChargeDuration = 1.5f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input Duration")
	float MaxInputDuration = 3.0f;

	
	UChargeAttackAbility();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

private:
	UPROPERTY()
	ACrashCharacter* OwnerCharacter;

	UPROPERTY()
	bool bAbilityInputRelease = false;

	UPROPERTY()
	FTimerHandle AbilityMaxDurationTimer;
	
	UPROPERTY()
	UAbilityTask_WaitInputRelease* AsyncWaitInputRelease;

	UPROPERTY()
	UAbilityTask_WaitGameplayEvent* AsyncWaitMontagePaused;
	
	const float ConstantBaseAttackDamage = AbilityDamage;

	UFUNCTION()
	void OnAttackMontagePaused(FGameplayEventData Payload);
	
	UFUNCTION()
	void OnAbilityInputRelease(float Duration);
	
	virtual void OnGameplayReceivedDamageEvent(FGameplayEventData Payload) override;

	UFUNCTION()
	void OnMaxAbilityDuration();
	
	UFUNCTION()
	void OnAttackMontageFinished(UAnimMontage* Montage, bool bInterrupted);
};
