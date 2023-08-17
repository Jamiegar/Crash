// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/Abiliities/Combat/AttackAbility.h"
#include "CounterAttackAbility.generated.h"

USTRUCT(Blueprintable)
struct FCounterAttackSoundData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Sound")
	USoundBase* CounterDodgeSoundEffect;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Sound")
	USoundBase* CounterReceivedSoundEffect;
};


UCLASS()
class CRASH_API UCounterAttackAbility : public UAttackAbility
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Counter")
	UAnimMontage* CounterAttack;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Counter")
	float CounterEffectDuration = 1.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Effects")
	FCounterAttackSoundData CounterAttackSoundData;
	
	UCounterAttackAbility();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

private:
	
	UPROPERTY()
	bool bShouldAttack = false;
	
	UPROPERTY()
	UAbilityTask_WaitGameplayEvent* WaitForCounterAttackEvent;
	
	UPROPERTY()
	UAbilityTask_WaitGameplayTagRemoved* WaitForCounterTagRemoved;

	UFUNCTION()
	void OnCounterAttackEventReceived(FGameplayEventData Payload);

	UFUNCTION()
	void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);
	
	UFUNCTION()
	void OnCounterTagRemoved();
	
	virtual void OnGameplayReceivedDamageEvent(FGameplayEventData Payload) override;
};
