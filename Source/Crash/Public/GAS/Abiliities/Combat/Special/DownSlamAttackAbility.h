// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/Abiliities/Combat/AttackAbility.h"
#include "DownSlamAttackAbility.generated.h"

class ACrashCharacter;
/**
 * 
 */
UCLASS()
class CRASH_API UDownSlamAttackAbility : public UAttackAbility
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Movement")
	float LaunchForce = 1200;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Movement")
	FVector LaunchDirection = FVector(0, 0, -1);

	UPROPERTY(EditDefaultsOnly, Category="Movement")
	UAnimMontage* AttackMontage;
	
	UDownSlamAttackAbility();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

private:
	UPROPERTY()
	ACrashCharacter* OwnerCharacter;
	
	UPROPERTY()
	UAbilityTask_WaitGameplayEvent* AsyncEventWaitUntilPause;

	UPROPERTY()
	UAbilityTask_WaitGameplayEvent* AsyncEventWaitUntilGrounded;

	UFUNCTION()
	void OnEventPauseMontage(FGameplayEventData Payload);

	UFUNCTION()
	void OnEventGrounded(FGameplayEventData Payload);
	
	UFUNCTION()
	void OnAttackMontageFinished(UAnimMontage* Montage, bool bInterrupted);

	virtual void OnGameplayReceivedDamageEvent(FGameplayEventData Payload) override;
};
