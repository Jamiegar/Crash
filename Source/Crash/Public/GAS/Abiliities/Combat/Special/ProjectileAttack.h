// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/Abiliities/Combat/AttackAbility.h"
#include "ProjectileAttack.generated.h"

class AProjectile;
/**
 * 
 */
UCLASS(Abstract)
class CRASH_API UProjectileAttack : public UAttackAbility
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Projectile")
	UAnimMontage* ProjectileAttackMontage = nullptr;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Projectile")
	TSubclassOf<AProjectile> Projectile = nullptr;

private:
	UPROPERTY()
	UAbilityTask_WaitGameplayEvent* AsyncProjectileFiredEvent;
	
public:
	UProjectileAttack();
	
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData) override;

private:
	void OnMontageFinished(UAnimMontage* Montage, bool bInterrupted);

	UFUNCTION()
	void OnGameplayReceivedFireProjectile(FGameplayEventData Payload);
	virtual void SpawnProjectile();
	
};
