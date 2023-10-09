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

/**********************************************************************************
* Attack abilities inherit from this abstract class
*
* NOTE !!
* Contains methods for:
* - Adding damage to target
* - Waiting for targets to receive damage
* - Applying knockback to target
* - Starting Hit-Stop based on damage target has received
* - Handles attack (Missed / Hit) SFX 
***********************************************************************************/
UCLASS(Abstract)
class CRASH_API UAttackAbility : public UCrashGameplayAbility
{
	GENERATED_BODY()

public:
	/* Constructor used to add default 'GameplayTags'*/
	UAttackAbility();

	/* Handles clean up of ability and stops any async 'GameplayTasks' */
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
		bool bReplicateEndAbility, bool bWasCancelled) override;

protected:

	/**********************************************************************************
	* Ability variables and data (Exposed in Blueprint)
	***********************************************************************************/

	/* Amount of damage that is given when ability damages target.
	 * (Used in percent damage calculation) */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Damage")
	float AbilityDamage = 1.0f;

	/* Scales the amount of knockback this ability gives to damaging target
	 * (Used in Knockback calculation as a scalar, note value of 0 will apply no knockback) */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Damage")
	float KnockbackScaling = 1.0f;

	/* Enables attack to stun target when damaged */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Damage")
	bool bAttackShouldStun = false;

	/* Can only be set when 'AttackShouldStun' == true. Data contains stun information e.g.
	 * (Duration, Shake Curve) */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Damage", meta=(EditCondition = bAttackShouldStun))
	UStunAbilityData* StunData;

	/* Data containing knockback information e.g.(Force, Duration, ect) */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Knockback")
	UKnockbackData* KnockbackData;

	/* Data with values for attack activation and Hit SFX, (Automatically played) */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Effects")
	FAttackAbilitySoundData SoundData;

	/**********************************************************************************
	* Variables and data (Not Exposed in Blueprint)
	***********************************************************************************/

	/* Payload data of target, it is set when target has been damaged */
	UPROPERTY()
	FGameplayEventData PayLoadEventData;

	/* Async damage task that will wait until target has been damaged after ability activation.
	 * If ability does not deal damage 'EndAbility' cleans up task
	 */
	UPROPERTY()
	UAbilityTask_WaitGameplayEvent* AsyncDamageTask;

	/* Async task waits for the Hit-Stop GameplayTag to be removed */
	UPROPERTY()
	UAbilityTask_WaitGameplayTagRemoved* AsyncHitStopTagRemoved;

	/**********************************************************************************
	* Helper methods
	***********************************************************************************/
	
	/* Activates Async task to wait for ability to damage a target */
	UFUNCTION()
	void WaitForDamageEffect();

	/* Callback for when a target has been damaged by ability */
	UFUNCTION()
	virtual void OnGameplayReceivedDamageEvent(FGameplayEventData Payload);

	/* Detect if target has counter attack */
	UFUNCTION()
	virtual bool DoesTargetHaveCounterAttack(ACrashCharacter* Target);

	/* Apply knockback to target using payload data */
	UFUNCTION()
	void ApplyKnockbackInstantToTarget(FGameplayEventData Payload);
	/* Apply knockback to target using 'PayLoadEventData'
	 * (It has already been set in 'OnGameplayReceivedDamageEvent') */
	void ApplyKnockbackInstantToTarget();

	/*
	 * NOTE !!
	 * Only used if 'bAttackShouldStun' == true
	 * Uses 'StunData' to give target stun */
	UFUNCTION()
	void ApplyStunToTarget(FGameplayEventData Payload);

	/* Adds Hit-Stop effect to Target and Instigator based on damage
	 * (More damage == longer Hit-Stop) */
	UFUNCTION()
	void ApplyHitStopInstant(FGameplayEventData Payload);

	/* If Target had counter this applies Hit-Stop instantly and activates 'AsyncHitStopTagRemoved' task with
	 * 'OnHitStopEndApplyCounter' as the callback. */
	UFUNCTION()
	void WaitForHitStopEndAndApplyCounter(FGameplayEventData Payload);

	/* The damage will be countered so the instigator now receives damage when the Hit-Stop ends*/
	UFUNCTION()
	void OnHitStopEndApplyCounter();

	/* Activates 'AsyncHitStopTagRemoved' and when Hit-stop ends calls 'OnHitStopEndApplyKnockback'*/
	UFUNCTION()
	void WaitForHitStopEndAndApplyKnockback(FGameplayEventData Payload);

	/* Applies Knockback 'ApplyKnockbackInstant' */
	UFUNCTION()
	virtual void OnHitStopEndApplyKnockback();
	
	/* Play SFX Helper functions */
	virtual void PlayMissedAttackSound();
	virtual void PlayContactHitAttackSound();
	virtual UAudioComponent* PlaySoundAtOwnerLocation(USoundBase* Sound);
};
