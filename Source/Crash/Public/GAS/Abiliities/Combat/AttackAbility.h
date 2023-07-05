// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/Abiliities/CrashGameplayAbility.h"
#include "AttackAbility.generated.h"

class UAbilityTask_WaitGameplayEvent;

UCLASS(Abstract)
class CRASH_API UAttackAbility : public UCrashGameplayAbility
{
	GENERATED_BODY()

public:
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
		bool bReplicateEndAbility, bool bWasCancelled) override;

	
protected:
	UPROPERTY(EditDefaultsOnly, Category="Damage")
	float AbilityDamage = 1.f;

	UPROPERTY(EditDefaultsOnly, Category="Damage")
	float KnockbackScaling = 0;
	
	UPROPERTY()
	UAbilityTask_WaitGameplayEvent* AsyncTask;
	
	UFUNCTION()
	void WaitForDamageEffect();
	
	UFUNCTION()
	virtual void OnGameplayReceivedDamageEvent(FGameplayEventData Payload);
};
