// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Effects/GroundedEffect.h"

UGroundedEffect::UGroundedEffect()
{
	InheritableOwnedTagsContainer.AddTag(FGameplayTag::RequestGameplayTag(FName("Player.State.Grounded")));
	RemoveGameplayEffectsWithTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Player.State.Airborne")));
	DurationPolicy = EGameplayEffectDurationType::Infinite;
}
