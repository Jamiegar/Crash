// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/Abiliities/CrashGameplayAbility.h"
#include "KnockBackAbility.generated.h"

class ACrashCharacter;
class UAbilityTask_WaitDelay;
class UKnockbackData;
class UAbilityTask_WaitGameplayEvent;
/**
 * 
 */
UCLASS()
class CRASH_API UKnockBackAbility : public UCrashGameplayAbility
{
	GENERATED_BODY()

public:
	UKnockBackAbility();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData) override;

	virtual void CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
		bool bReplicateCancelAbility) override;

	FVector KnockbackDirection;

	UPROPERTY()
	UAnimMontage* GetUpMontage;

private:
	UPROPERTY()
	ACrashCharacter* TargetCharacter;

	UPROPERTY()
	UKnockbackData* KnockbackData;
	
	UPROPERTY()
	UAbilityTask_WaitGameplayEvent* AsyncEventKnockbackData;

	UFUNCTION()
	void OnReceivedKnockbackData(FGameplayEventData Payload);

	UPROPERTY()
	UAbilityTask_WaitGameplayEvent* AsyncEventWaitUntilGrounded;

	UFUNCTION()
	void OnCharacterGrounded(FGameplayEventData Payload);

	UPROPERTY()
	UAbilityTask_WaitDelay* WaitAsyncKncokbackEnd;

	UFUNCTION()
	void OnKnockbackGroundedFinished();

	UFUNCTION()
	void GetupMontageFinished(UAnimMontage* Montage, bool bInterrupted);

};
