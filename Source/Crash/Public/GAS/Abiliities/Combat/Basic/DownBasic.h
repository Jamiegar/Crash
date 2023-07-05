// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/Abiliities/Combat/BasicAttack.h"
#include "DownBasic.generated.h"

/**
 * 
 */
UCLASS()
class CRASH_API UDownBasic : public UBasicAttack
{
	GENERATED_BODY()

	UDownBasic();
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void OnGameplayReceivedDamageEvent(FGameplayEventData Payload) override;
	
};
