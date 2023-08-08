// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/Abiliities/Combat/AttackAbility.h"
#include "LongRangeAttackAbility.generated.h"

class ACrashCharacter;
/**
 * 
 */
UCLASS()
class CRASH_API ULongRangeAttackAbility : public UAttackAbility
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category="Attack")
	UAnimMontage* AttackMontage;
	
	ULongRangeAttackAbility();
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	void OnAttackMontageEnd(UAnimMontage* Montage, bool bInterrupted);

	virtual  void OnGameplayReceivedDamageEvent(FGameplayEventData Payload) override;

private:
	UPROPERTY()
	ACrashCharacter* OwnerCharacter = nullptr;
};
