// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/CrashAbilitySystemGlobals.h"
#include "GAS/CrashGameplayEffectContext.h"
#include "GameplayEffectTypes.h"
#include "Abilities/GameplayAbility.h"

FGameplayEffectContext* UCrashAbilitySystemGlobals::AllocGameplayEffectContext() const
{
	return new FCrashGameplayEffectContext();
}



