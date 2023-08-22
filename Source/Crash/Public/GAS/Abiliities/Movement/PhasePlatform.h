// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/Abiliities/CrashGameplayAbility.h"
#include "PhasePlatform.generated.h"

class UAbilityTask_WaitInputPress;
class UAbilityTask_WaitInputRelease;
/**
 * 
 */
UCLASS()
class CRASH_API UPhasePlatform : public UCrashGameplayAbility
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Platform Trace")
	float DownTraceDistance = 250;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Down Force")
	float DownForce = 600.f;
	
	UPROPERTY()
	UAbilityTask_WaitInputRelease* AsyncWaitInputReleased;
	
	FTimerHandle PlatformDetectionTimerHandle;
 
	UPhasePlatform();
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	void DetectPlatform();
	
	UFUNCTION()
	void OnAbilityInputReleased(float DownDuration);
	
};
