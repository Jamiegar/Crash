// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/Abiliities/CrashGameplayAbility.h"
#include "RespawnAbility.generated.h"


class UAbilityTask_WaitGameplayTagRemoved;

UCLASS()
class CRASH_API URespawnAbility : public UCrashGameplayAbility
{
	GENERATED_BODY()

public:
	URespawnAbility();
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData) override;

private:
	UPROPERTY()
	UAbilityTask_WaitGameplayTagRemoved* AsyncWaitUntilTagRemoved;

	UFUNCTION()
	void OnDeathTagRemoved();
};
