// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Abiliities/Combat/AttackAbility.h"

#include "AbilitySystemGlobals.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "GAS/CrashGameplayTags.h"
#include "GAS/Effects/DamageBasicInstant.h"
#include "Crash/Public/GAS/Abiliities/Combat/Damage/Data/StunAbilityData.h"
#include "GAS/Effects/KnockbackCalculationEffect.h"
#include "AbilitySystemComponent.h"
#include "GAS/Abiliities/Combat/Damage/StunAbility.h"
#include "GAS/Abiliities/Combat/Damage/Data/KnockbackData.h"

void UAttackAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
	
	if(AsyncDamageTask)
		if(AsyncDamageTask->IsActive())
			AsyncDamageTask->EndTask();
}

void UAttackAbility::WaitForDamageEffect()
{
	const FCrashGameplayTags& GameTags = FCrashGameplayTags::Get();
	AsyncDamageTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, GameTags.PlayerDamaged, nullptr, true);
	AsyncDamageTask->EventReceived.AddUniqueDynamic(this, &UAttackAbility::OnGameplayReceivedDamageEvent);
	AsyncDamageTask->Activate();
}

void UAttackAbility::OnGameplayReceivedDamageEvent(FGameplayEventData Payload)
{
	const FCrashGameplayTags& GameTags = FCrashGameplayTags::Get();
	const FGameplayEffectSpecHandle Handle = MakeEffectSpecHandleFromAbility(UDamageBasicInstant::StaticClass());

	if(!Handle.IsValid())
		return;
	
	FGameplayEffectSpec* Spec = Handle.Data.Get();
	Spec->SetSetByCallerMagnitude(GameTags.PlayerDamaged, AbilityDamage); //Sends the Ability damage to the custom damage execute calculation 
	ApplyAbilityTagsToGameplayEffectSpec(*Handle.Data.Get(), GetCurrentAbilitySpec()); 
	auto ActiveGameplayEffectHandles = ApplyGameplayEffectSpecToTargetFromAbility(Handle, Payload.TargetData);
}

void UAttackAbility::ApplyKnockbackToTarget(FGameplayEventData Payload)
{
	const FGameplayEffectSpecHandle Handle = MakeEffectSpecHandleFromAbility(UKnockbackCalculationEffect::StaticClass());
	
	FGameplayEffectSpec* Spec = Handle.Data.Get();
	Spec->SetSetByCallerMagnitude("Player.Damaged.Knockback", KnockbackScaling);

	ApplyAbilityTagsToGameplayEffectSpec(*Handle.Data.Get(), GetCurrentAbilitySpec());
	
	auto ActiveGameplayEffectHandles= ApplyGameplayEffectSpecToTargetFromAbility(Handle, Payload.TargetData);

	FGameplayEventData EventKnockbackData;
	EventKnockbackData.Instigator = GetActorInfo().AvatarActor.Get();
	EventKnockbackData.Target = Payload.Target;

	if(KnockbackData)
		EventKnockbackData.OptionalObject = KnockbackData;
		
	UAbilitySystemComponent* TargetComponent = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Payload.Target);
	TargetComponent->HandleGameplayEvent(FGameplayTag::RequestGameplayTag("Event.Data.Knockback"), &EventKnockbackData);
}

void UAttackAbility::ApplyStunToTarget(FGameplayEventData Payload)
{
	UAbilitySystemComponent* TargetComponent = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Payload.Target);

	FGameplayAbilitySpec StunSpec = FGameplayAbilitySpec(UStunAbility::StaticClass());
	TargetComponent->GiveAbility(StunSpec);
	
	if(StunData != nullptr && bAttackShouldStun)
	{
		FGameplayEventData StunPayloadData;
		StunPayloadData.Instigator = CastChecked<AActor>(GetActorInfo().AvatarActor);
		StunPayloadData.Target = Payload.Target;
		StunPayloadData.OptionalObject = StunData;

		const FGameplayTag StunEventTag = FGameplayTag::RequestGameplayTag("Event.Data.Stun");
		TargetComponent->HandleGameplayEvent(StunEventTag, &StunPayloadData);
	}
}
