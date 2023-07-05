// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttackAbility.h"
#include "ComboAttack.generated.h"

class UAbilityTask_WaitDelay;
class UAbilityTask_WaitInputPress;
/**
 * 
 */
UCLASS()
class CRASH_API UComboAttack : public UAttackAbility
{
	GENERATED_BODY()

public:
	UComboAttack();
	virtual void PostInitProperties() override;

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
		bool bReplicateEndAbility, bool bWasCancelled) override;

private:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Combos", meta=(AllowPrivateAccess = true))
	TArray<UAnimMontage*> ComboMontages;

	UPROPERTY()
	UAbilityTask_WaitDelay* WaitDelayAsync;

	UPROPERTY()
	int ComboCount;

	UFUNCTION()
	void OnMontageFinished(UAnimMontage* Montage, bool bInterrupted);

	UFUNCTION()
	void OnWaitFinished();
	
};
