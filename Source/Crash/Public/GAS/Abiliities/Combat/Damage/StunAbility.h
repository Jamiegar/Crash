﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/Abiliities/CrashGameplayAbility.h"
#include "StunAbility.generated.h"

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

private:
	UPROPERTY()
	UAbilityTask_WaitGameplayEvent* AsyncStunDataEvent;

	UFUNCTION()
	void OnReceivedStunData(FGameplayEventData Payload);
};