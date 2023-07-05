// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/Abiliities/CrashGameplayAbility.h"
#include "KnockBackAbility.generated.h"

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

	FVector KnockbackDirection; 
};
