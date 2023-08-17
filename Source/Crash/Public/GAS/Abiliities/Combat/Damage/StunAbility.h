// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/Abiliities/CrashGameplayAbility.h"
#include "StunAbility.generated.h"

class ACrashCharacter;
class UAbilityTask_WaitDelay;
class UAbilityTask_WaitGameplayEvent;
/**
 * 
 */
UCLASS()
class CRASH_API UStunAbility : public UCrashGameplayAbility
{
	GENERATED_BODY()

public:
	UStunAbility();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData) override;

	virtual void CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
		bool bReplicateCancelAbility) override;

private:
	UPROPERTY()
	ACrashCharacter* TargetCharacter;
	
	UPROPERTY()
	UAbilityTask_WaitGameplayEvent* AsyncStunDataEvent;

	UPROPERTY()
	UAbilityTask_WaitDelay* AsyncStunDuration;

	UPROPERTY()
	FVector StartingMeshPosition;
	
	UFUNCTION()
	void OnReceivedStunData(FGameplayEventData Payload);
	
	UFUNCTION()
	void OnStunDurationFinished();

	UFUNCTION()
	void TimelineUpdate(float interpolatedValue);
};
