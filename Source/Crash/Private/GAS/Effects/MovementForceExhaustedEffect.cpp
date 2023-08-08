// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Effects/MovementForceExhaustedEffect.h"

UMovementForceExhaustedEffect::UMovementForceExhaustedEffect()
{
	DurationPolicy = EGameplayEffectDurationType::Infinite;

	InheritableGameplayEffectTags.AddTag(FGameplayTag::RequestGameplayTag("Player.MovementAction.DashExhausted"));
	InheritableOwnedTagsContainer.AddTag(FGameplayTag::RequestGameplayTag("Player.MovementAction.DashExhausted"));
}
