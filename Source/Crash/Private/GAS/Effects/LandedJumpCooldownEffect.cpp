// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Effects/LandedJumpCooldownEffect.h"

ULandedJumpCooldownEffect::ULandedJumpCooldownEffect()
{
	DurationPolicy = EGameplayEffectDurationType::HasDuration;
	DurationMagnitude = FGameplayEffectModifierMagnitude(0.2);
	
	InheritableOwnedTagsContainer.AddTag(FGameplayTag::RequestGameplayTag("Cooldown.Jump"));
	InheritableGameplayEffectTags.AddTag(FGameplayTag::RequestGameplayTag("Cooldown.Jump"));
}
