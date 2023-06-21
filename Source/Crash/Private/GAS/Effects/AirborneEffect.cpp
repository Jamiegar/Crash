// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Effects/AirborneEffect.h"


UAirborneEffect::UAirborneEffect()
{
	DurationPolicy = EGameplayEffectDurationType::Infinite;
	InheritableOwnedTagsContainer.AddTag(FGameplayTag::RequestGameplayTag(FName("Player.State.Airborne")));
	RemoveGameplayEffectsWithTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Player.State.Grounded")));
}



