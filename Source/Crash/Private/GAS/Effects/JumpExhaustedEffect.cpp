// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Effects/JumpExhaustedEffect.h"

UJumpExhaustedEffect::UJumpExhaustedEffect()
{
	DurationPolicy = EGameplayEffectDurationType::Infinite;

	InheritableGameplayEffectTags.AddTag(FGameplayTag::RequestGameplayTag("Player.MovementAction.JumpExhausted"));
	InheritableOwnedTagsContainer.AddTag(FGameplayTag::RequestGameplayTag("Player.MovementAction.JumpExhausted"));
}
