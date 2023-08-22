// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/Abiliities/CrashGameplayAbility.h"
#include "KnockBackAbility.generated.h"

class ACrashCharacter;
class UAbilityTask_WaitDelay;
class UKnockbackData;
class UAbilityTask_WaitGameplayEvent;

USTRUCT(Blueprintable)
struct FKnockbackSoundData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Sound")
	USoundBase* KnockbackStartSoundEffect;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Sound")
	USoundBase* KnockbackEndSoundEffect;
};


UCLASS()
class CRASH_API UKnockBackAbility : public UCrashGameplayAbility
{
	GENERATED_BODY()

public:
	UKnockBackAbility();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData) override;

	virtual void CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
		bool bReplicateCancelAbility) override;

	FVector KnockbackDirection;

	UPROPERTY()
	UAnimMontage* GetUpMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Effects")
	FKnockbackSoundData KnockbackSoundData;

private:
	UPROPERTY()
	ACrashCharacter* TargetCharacter;

	UPROPERTY()
	UKnockbackData* KnockbackData;

	UPROPERTY()
	float KnockbackMag;
	
	UPROPERTY()
	UAbilityTask_WaitGameplayEvent* AsyncEventKnockbackData;

	UPROPERTY()
	UAudioComponent* KnockbackAudio;

	UPROPERTY()
	UAbilityTask_WaitGameplayEvent* AsyncEventWaitUntilGrounded;

	UPROPERTY()
	UAbilityTask_WaitDelay* WaitAsyncKncokbackEnd;

	UFUNCTION()
	void OnReceivedKnockbackData(FGameplayEventData Payload);

	UFUNCTION()
	void OnCharacterGrounded(FGameplayEventData Payload);
	
	UFUNCTION()
	void OnKnockbackGroundedFinished();

	UFUNCTION()
	void GetupMontageFinished(UAnimMontage* Montage, bool bInterrupted);

	UFUNCTION()
	void OnCharacterFallingDown();
};
