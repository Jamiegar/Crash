// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/Abiliities/CrashGameplayAbility.h"
#include "BlockAbility.generated.h"

class UNiagaraComponent;
class UNiagaraSystem;
class UAbilityTask_WaitInputRelease;

UCLASS()
class CRASH_API UBlockAbility : public UCrashGameplayAbility
{
	GENERATED_BODY()

public:
	UBlockAbility();
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData) override;

	virtual void CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
		bool bReplicateCancelAbility) override;

private:
	UPROPERTY()
	UAbilityTask_WaitInputRelease* AsyncWaitInputRelease;

	UFUNCTION()
	void OnInputRelease(float TimeHeld);

	UPROPERTY(EditDefaultsOnly, Category="Shield")
	UStaticMesh* ShieldMesh;

	UPROPERTY(EditDefaultsOnly, Category="Shield")
	UNiagaraSystem* NiagaraShieldVFX;

	UPROPERTY()
	UNiagaraComponent* ActiveShieldVFX;
	
	UPROPERTY()
	UStaticMeshComponent* ShieldMeshComponent;
};
