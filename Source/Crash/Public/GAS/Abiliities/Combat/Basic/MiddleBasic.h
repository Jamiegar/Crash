// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/Abiliities/Combat/BasicAttack.h"
#include "MiddleBasic.generated.h"

/**
 * 
 */
UCLASS()
class CRASH_API UMiddleBasic : public UBasicAttack
{
	GENERATED_BODY()

	UMiddleBasic();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

};
