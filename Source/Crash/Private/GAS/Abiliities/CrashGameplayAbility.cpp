// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Abiliities/CrashGameplayAbility.h"

#include "AbilitySystemComponent.h"

UCrashGameplayAbility::UCrashGameplayAbility()
{
}

void UCrashGameplayAbility::TryActivateAbilityOnSpawn(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) const
{
	if(ActorInfo && !Spec.IsActive() && ActivationPolicy == OnSpawn)
	{
		if(UAbilitySystemComponent* ASC = ActorInfo->AbilitySystemComponent.Get())
		{
			ASC->TryActivateAbility(Spec.Handle);
		}
	}
}

void UCrashGameplayAbility::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnGiveAbility(ActorInfo, Spec);

	TryActivateAbilityOnSpawn(ActorInfo, Spec);
	
}

FGameplayEffectSpecHandle UCrashGameplayAbility::MakeEffectSpecHandleFromAbility(const TSubclassOf<UGameplayEffect> EffectClass) const
{
	const FGameplayEffectSpecHandle Handle =
		MakeOutgoingGameplayEffectSpec(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), EffectClass);

	return Handle;
}

TArray<FActiveGameplayEffectHandle> UCrashGameplayAbility::ApplyGameplayEffectSpecToTargetFromAbility(FGameplayEffectSpecHandle SpecHandle, FGameplayAbilityTargetDataHandle& TargetData) const
{
	auto ActiveGameplayEffectHandles =
		ApplyGameplayEffectSpecToTarget(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), SpecHandle, TargetData);

	return ActiveGameplayEffectHandles;
}




