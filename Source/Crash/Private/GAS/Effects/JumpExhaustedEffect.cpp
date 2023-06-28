// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Effects/JumpExhaustedEffect.h"

UJumpExhaustedEffect::UJumpExhaustedEffect()
{
	DurationPolicy = EGameplayEffectDurationType::Infinite;
	InheritableOwnedTagsContainer.AddTag(FGameplayTag::RequestGameplayTag(FName("Player.MovementAction.JumpExhausted")));
}
