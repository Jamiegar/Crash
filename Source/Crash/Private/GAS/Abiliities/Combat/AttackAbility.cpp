// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Abiliities/Combat/AttackAbility.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "GAS/CrashGameplayEffectContext.h"
#include "GAS/CrashGameplayTags.h"
#include "GAS/Effects/DamageBasicInstant.h"

void UAttackAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
	
	if(AsyncTask)
		if(AsyncTask->IsActive())
			AsyncTask->EndTask();
}

void UAttackAbility::WaitForDamageEffect()
{
	const FCrashGameplayTags& GameTags = FCrashGameplayTags::Get();
	AsyncTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, GameTags.PlayerDamaged, nullptr, true);
	AsyncTask->EventReceived.AddUniqueDynamic(this, &UAttackAbility::OnGameplayReceivedDamageEvent);
	AsyncTask->Activate();
}

void UAttackAbility::OnGameplayReceivedDamageEvent(FGameplayEventData Payload)
{
	const FCrashGameplayTags& GameTags = FCrashGameplayTags::Get();

	const FGameplayEffectSpecHandle Handle = MakeEffectSpecHandleFromAbility(UDamageBasicInstant::StaticClass());

	if(!Handle.IsValid())
		return;
	
	FGameplayEffectSpec* Spec = Handle.Data.Get();
	Spec->SetSetByCallerMagnitude(GameTags.PlayerDamaged, AbilityDamage);
	
	ApplyAbilityTagsToGameplayEffectSpec(*Handle.Data.Get(), GetCurrentAbilitySpec());
	
	auto ActiveGameplayEffectHandles = ApplyGameplayEffectSpecToTargetFromAbility(Handle, Payload.TargetData);
}
