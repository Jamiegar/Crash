// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttackAbility.h"
#include "BasicAttack.generated.h"


class UAbilityTask_WaitGameplayEvent;

UCLASS(Abstract)
class CRASH_API UBasicAttack : public UAttackAbility
{
	GENERATED_BODY()

public:
	UBasicAttack();
	virtual void PostInitProperties() override;

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData) override;


	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
		bool bReplicateEndAbility, bool bWasCancelled) override;


protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="BasicAttack")
	UAnimMontage* AttackMontage;

	UFUNCTION()
	virtual void OnMontageFinished(UAnimMontage* Montage, bool bInterrupted);

	virtual void OnGameplayReceivedDamageEvent(FGameplayEventData Payload) override;
};


