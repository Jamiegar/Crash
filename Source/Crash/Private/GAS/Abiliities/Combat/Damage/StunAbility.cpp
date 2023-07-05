// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Abiliities/Combat/Damage/StunAbility.h"

#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "GAS/CrashGameplayTags.h"

UStunAbility::UStunAbility()
{
	ActivationPolicy = ECrashActivationPolicy::OnSpawn;
}

void UStunAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	CommitCrashAbility();
	
	AsyncStunDataEvent = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, CrashGameplayTags::GetGameplayTagFromName("Event.StunData"));
	AsyncStunDataEvent->OnlyTriggerOnce = true;
	AsyncStunDataEvent->EventReceived.AddUniqueDynamic(this, &UStunAbility::OnReceivedStunData);

	UE_LOG(LogTemp, Warning, TEXT("Activated Stun"));
}

void UStunAbility::OnReceivedStunData(FGameplayEventData Payload)
{
	UE_LOG(LogTemp, Warning, TEXT("Received Stun Data"));
}
