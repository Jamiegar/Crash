// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Abiliities/Combat/Special/MovementAttackAbility.h"

#include "Characters/CrashCharacter.h"

UMovementAttackAbility::UMovementAttackAbility()
{
	AbilityInputID = EAbilityInputID::SpecialAttackUp;
	AbilityDamage = 6;
	KnockbackScaling = 15;
}

void UMovementAttackAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	ACrashCharacter* Character = CastChecked<ACrashCharacter>(GetActorInfo().OwnerActor);
	Character->LaunchCharacter(LaunchDirection * LaunchForce, false, true);

	
	FOnMontageEnded Delegate = FOnMontageEnded::CreateUObject(this, &UMovementAttackAbility::OnMovementMontageEnded);
	PlayAnimationMontageToOwningActor(AttackMontage, Delegate);

	WaitForDamageEffect();
}

void UMovementAttackAbility::OnMovementMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	EndCrashAbility();
}

void UMovementAttackAbility::OnGameplayReceivedDamageEvent(FGameplayEventData Payload)
{
	Super::OnGameplayReceivedDamageEvent(Payload);
	WaitForHitStopEndAndApplyKnockback(Payload);
}
