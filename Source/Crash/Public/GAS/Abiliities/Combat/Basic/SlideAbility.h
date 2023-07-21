// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/Abiliities/CrashGameplayAbility.h"
#include "GAS/Abiliities/Combat/AttackAbility.h"
#include "SlideAbility.generated.h"

/**
 * 
 */
UCLASS()
class CRASH_API USlideAbility : public UAttackAbility
{
	GENERATED_BODY()

public:
	USlideAbility();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData) override;

private:
	UPROPERTY(EditDefaultsOnly, Category="Slide")
	UAnimMontage* SlideMontage;

	UFUNCTION()
	void OnMontageFinished(UAnimMontage* AnimMontage, bool bInterrupted);

	virtual void OnGameplayReceivedDamageEvent(FGameplayEventData Payload) override;
};
