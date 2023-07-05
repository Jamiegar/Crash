// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/Abiliities/Combat/BasicAttack.h"
#include "UpBasic.generated.h"

/**
 * 
 */
UCLASS()
class CRASH_API UUpBasic : public UBasicAttack
{
	GENERATED_BODY()

	UUpBasic();
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
};
