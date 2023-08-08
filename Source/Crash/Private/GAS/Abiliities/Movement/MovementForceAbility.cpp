// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Abiliities/Movement/MovementForceAbility.h"
#include "Characters/CrashCharacter.h"
#include "GAS/Effects/MovementForceExhaustedEffect.h"

UMovementForceAbility::UMovementForceAbility()
{
	AbilityInputID = EAbilityInputID::SpecialAttackUp;

	ActivationOwnedTags.AddTag(FGameplayTag::RequestGameplayTag("Player.MovementAction.Dash"));

	ActivationBlockedTags.AddTag(FGameplayTag::RequestGameplayTag("Player.MovementAction.DashExhausted"));
	ActivationBlockedTags.AddTag(FGameplayTag::RequestGameplayTag("Player.MovementAction.Dash"));
	ActivationBlockedTags.AddTag(FGameplayTag::RequestGameplayTag("Player.Attack"));
	ActivationBlockedTags.AddTag(FGameplayTag::RequestGameplayTag("Player.State.Blocking"));
	ActivationBlockedTags.AddTag(FGameplayTag::RequestGameplayTag("Cooldown.Jump"));
}

void UMovementForceAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if(!CommitCrashAbility())
		return;

	PlayAnimationMontageToOwningActor(MovementMontage);
	
	ACrashCharacter* Character = CastChecked<ACrashCharacter>(GetActorInfo().OwnerActor.Get());
	Character->LaunchCharacter(LaunchDirection * LaunchForce, false, true);

	const FGameplayEffectSpecHandle ExhaustedEffectHandle = MakeOutgoingGameplayEffectSpec(UMovementForceExhaustedEffect::StaticClass());
	auto ActiveEffects = ApplyGameplayEffectSpecToOwnerFromAbility(ExhaustedEffectHandle);
	
	EndCrashAbility();
}
