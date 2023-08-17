// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/Abiliities/CrashGameplayAbility.h"
#include "AttackAbility.generated.h"

class ACrashCharacter;
class UAbilityTask_WaitGameplayTagRemoved;
class UKnockbackData;
class UStunAbilityData;
class UAbilityTask_WaitGameplayEvent;

USTRUCT(Blueprintable)
struct FAttackAbilitySoundData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Sound")
	USoundBase* MissedAttackSound;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Sound")
	USoundBase* ContactHitSound; 
};


UCLASS(Abstract)
class CRASH_API UAttackAbility : public UCrashGameplayAbility
{
	GENERATED_BODY()

public:
	UAttackAbility();
	
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
		bool bReplicateEndAbility, bool bWasCancelled) override;

	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Damage")
	float AbilityDamage = 1.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Damage")
	float KnockbackScaling = 1.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Damage")
	bool bAttackShouldStun = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Damage", meta=(EditCondition = bAttackShouldStun))
	UStunAbilityData* StunData;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Knockback")
	UKnockbackData* KnockbackData;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Effects")
	FAttackAbilitySoundData SoundData;
	
	UPROPERTY()
	FGameplayEventData PayLoadEventData;
	
	UPROPERTY()
	UAbilityTask_WaitGameplayEvent* AsyncDamageTask;

	UPROPERTY()
	UAbilityTask_WaitGameplayTagRemoved* AsyncHitStopTagRemoved;
	
	UFUNCTION()
	void WaitForDamageEffect();
	
	UFUNCTION()
	virtual void OnGameplayReceivedDamageEvent(FGameplayEventData Payload);

	UFUNCTION()
	virtual bool DoesTargetHaveCounterAttack(ACrashCharacter* Target);
	
	UFUNCTION()
	void ApplyKnockbackInstantToTarget(FGameplayEventData Payload);
	void ApplyKnockbackInstantToTarget();

	UFUNCTION()
	void ApplyStunToTarget(FGameplayEventData Payload);

	UFUNCTION()
	void ApplyHitStopInstant(FGameplayEventData Payload);

	UFUNCTION()
	void WaitForHitStopEndAndApplyCounter(FGameplayEventData Payload);

	UFUNCTION()
	void OnHitStopEndApplyCounter();
	
	UFUNCTION()
	void WaitForHitStopEndAndApplyKnockback(FGameplayEventData Payload);

	UFUNCTION()
	virtual void OnHitStopEndApplyKnockback();

	
	virtual void PlayMissedAttackSound();
	virtual void PlayContactHitAttackSound();
	virtual UAudioComponent* PlaySoundAtOwnerLocation(USoundBase* Sound);

	
};
