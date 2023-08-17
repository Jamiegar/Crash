// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttackAbility.h"
#include "ComboAttack.generated.h"

class UAbilityTask_WaitInputPress;
class UAbilityTask_PlayMontageAndWait;


UCLASS(Abstract)
class CRASH_API UComboAttack : public UAttackAbility
{
	GENERATED_BODY()

public:
	UComboAttack();
	virtual void PostInitProperties() override;

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
		bool bReplicateEndAbility, bool bWasCancelled) override;

	virtual void OnGameplayReceivedDamageEvent(FGameplayEventData Payload) override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Combos", meta=(AllowPrivateAccess = true))
	TArray<UAnimMontage*> ComboMontages;

private:
	UPROPERTY()
	float DefaultAttackDamage;
	
	UPROPERTY()
	UAbilityTask_PlayMontageAndWait* MontageWaitTask;

	UPROPERTY()
	UAbilityTask_WaitGameplayEvent* AsyncComboOpenEvent;

	UPROPERTY()
	UAbilityTask_WaitGameplayEvent* AsyncComboCloseEvent;

	UPROPERTY()
	UAbilityTask_WaitGameplayEvent* AsyncComboResetEvent;

	UPROPERTY()
	UAbilityTask_WaitInputPress* AsyncComboInputPressed;
	
	UPROPERTY()
	int ComboCount;

	UPROPERTY()
	bool bDidRequestCombo = false;

	UPROPERTY()
	bool bIsComboWindowOpen = false;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Combos")
	bool DoesNextAttackExist() { return ComboMontages.IsValidIndex(ComboCount + 1); }

	UFUNCTION(BlueprintCallable, Category="Combos")
	void NextAttackInSequence();
	
	UFUNCTION(BlueprintCallable, Category="Combos")
	UAnimMontage* GetMontageToPlay();

	UFUNCTION()
	void OnMontageFinished();

	UFUNCTION()
	void OnGameplayReceivedOnComboOpen(FGameplayEventData Payload);

	UFUNCTION()
	void OnGameplayReceivedComboClose(FGameplayEventData Payload);

	UFUNCTION()
	void OnGameplayReceivedComboRest(FGameplayEventData Payload);

	UFUNCTION()
	void OnAbilityInputPressed(float TimeWaited);
	
};

