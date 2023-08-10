// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Abiliities/Combat/Special/CounterAttackAbility.h"

#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayTag.h"
#include "GAS/Effects/Damaging/CounterAttackEffect.h"

UCounterAttackAbility::UCounterAttackAbility()
{
	ActivationOwnedTags.AddTag(FGameplayTag::RequestGameplayTag("Player.State.Invincible"));

	AbilityInputID = EAbilityInputID::SpecialAttack;
	AbilityDamage = 7;
	KnockbackScaling = 15;
}

void UCounterAttackAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if(!CommitCrashAbility())
		return;

	const FGameplayEffectSpecHandle CounterEffectSpecHandle = MakeOutgoingGameplayEffectSpec(UCounterAttackEffect::StaticClass());
	ApplyAbilityTagsToGameplayEffectSpec(*CounterEffectSpecHandle.Data.Get(), GetCurrentAbilitySpec());
	CounterEffectSpecHandle.Data->SetDuration(CounterEffectDuration, true);
	auto ActiveEffects = ApplyGameplayEffectSpecToOwnerFromAbility(CounterEffectSpecHandle);

	const FGameplayTag ActivateCounterAttackEventTag = FGameplayTag::RequestGameplayTag("Event.Counter");
	WaitForCounterAttackEvent = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, ActivateCounterAttackEventTag, nullptr, true);
	WaitForCounterAttackEvent->EventReceived.AddUniqueDynamic(this, &UCounterAttackAbility::OnCounterAttackEventReceived);
	WaitForCounterAttackEvent->Activate();

	const FGameplayTag CounterTag = FGameplayTag::RequestGameplayTag("Player.State.Counter");
	WaitForCounterTagRemoved = UAbilityTask_WaitGameplayTagRemoved::WaitGameplayTagRemove(this, CounterTag, nullptr, true);
	WaitForCounterTagRemoved->Removed.AddUniqueDynamic(this, &UCounterAttackAbility::OnCounterTagRemoved);
	WaitForCounterTagRemoved->Activate();
}

void UCounterAttackAbility::OnCounterAttackEventReceived(FGameplayEventData Payload)
{
	bShouldAttack = true;

	FOnMontageEnded OnMontageEnded = FOnMontageEnded::CreateUObject(this, &UCounterAttackAbility::OnAttackMontageEnded);
	PlayAnimationMontageToOwningActor(CounterAttack, OnMontageEnded);

	WaitForDamageEffect();
}

void UCounterAttackAbility::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	EndCrashAbility();
}

void UCounterAttackAbility::OnCounterTagRemoved()
{
	if(!bShouldAttack)
		EndCrashAbility();
}

void UCounterAttackAbility::OnGameplayReceivedDamageEvent(FGameplayEventData Payload)
{
	Super::OnGameplayReceivedDamageEvent(Payload);
	WaitForHitStopEndAndApplyKnockback(Payload);
}
