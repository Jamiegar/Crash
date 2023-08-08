// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Effects/Damaging/HitStopEffect.h"
#include "GAS/Abiliities/Combat/Damage/HitStopAbility.h"

UHitStopEffect::UHitStopEffect()
{
	InheritableGameplayEffectTags.AddTag(FGameplayTag::RequestGameplayTag("Player.Damaged.HitStop"));
	InheritableOwnedTagsContainer.AddTag(FGameplayTag::RequestGameplayTag("Player.Damaged.HitStop"));
	
	DurationPolicy = EGameplayEffectDurationType::HasDuration;
	DurationMagnitude = FGameplayEffectModifierMagnitude(0.5f);
	
	FGameplayAbilitySpecDef HitStopAbilitySpecDef;
	HitStopAbilitySpecDef.Ability = UHitStopAbility::StaticClass();
	HitStopAbilitySpecDef.RemovalPolicy = EGameplayEffectGrantedAbilityRemovePolicy::CancelAbilityImmediately;

	GrantedAbilities.Add(HitStopAbilitySpecDef);
}
