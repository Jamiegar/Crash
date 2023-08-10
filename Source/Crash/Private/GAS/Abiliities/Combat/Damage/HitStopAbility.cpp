// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Abiliities/Combat/Damage/HitStopAbility.h"

UHitStopAbility::UHitStopAbility()
{
	ActivationPolicy = ECrashActivationPolicy::OnSpawn;
}

void UHitStopAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if(!CommitCrashAbility())
		return;

	UE_LOG(LogTemp, Warning, TEXT("Start HitStop"));
	GetActorInfo().OwnerActor.Get()->CustomTimeDilation = 0.0;
}

void UHitStopAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
	GetActorInfo().OwnerActor.Get()->CustomTimeDilation = 1;
}



