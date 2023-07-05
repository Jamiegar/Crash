// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Abiliities/Combat/Damage/KnockBackAbility.h"
#include "Characters/CrashCharacter.h"

UKnockBackAbility::UKnockBackAbility()
{
	ActivationPolicy = OnSpawn;
	
}

void UKnockBackAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if(const bool CanActivate = CommitAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo()); !CanActivate)
		CancelAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), false);

	if(ACrashCharacter* Character = Cast<ACrashCharacter>(GetActorInfo().AvatarActor))
	{
		
	}
	
	
}
