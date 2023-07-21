// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/Abiliities/CrashGameplayAbility.h"
#include "AttackAbility.generated.h"

class UKnockbackData;
class UStunAbilityData;
class UAbilityTask_WaitGameplayEvent;

UCLASS(Abstract)
class CRASH_API UAttackAbility : public UCrashGameplayAbility
{
	GENERATED_BODY()

public:
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
		bool bReplicateEndAbility, bool bWasCancelled) override;

	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Damage")
	float AbilityDamage = 1.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Damage")
	float KnockbackScaling = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Damage")
	bool bAttackShouldStun = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Damage", meta=(EditCondition = bAttackShouldStun))
	UStunAbilityData* StunData;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Knockback")
	UKnockbackData* KnockbackData;
	
	UPROPERTY()
	UAbilityTask_WaitGameplayEvent* AsyncDamageTask;
	
	UFUNCTION()
	void WaitForDamageEffect();
	
	UFUNCTION()
	virtual void OnGameplayReceivedDamageEvent(FGameplayEventData Payload);

	UFUNCTION()
	void ApplyKnockbackToTarget(FGameplayEventData Payload);

	UFUNCTION()
	void ApplyStunToTarget(FGameplayEventData Payload);
};
