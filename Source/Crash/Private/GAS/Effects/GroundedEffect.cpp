// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Effects/GroundedEffect.h"

#include "GAS/CrashAttributeSet.h"

UGroundedEffect::UGroundedEffect()
{
	DurationPolicy = EGameplayEffectDurationType::Instant;
	InheritableOwnedTagsContainer.AddTag(FGameplayTag::RequestGameplayTag(FName("Player.State.Grounded")));
	RemoveGameplayEffectsWithTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Player.State.Airborne")));
	RemoveGameplayEffectsWithTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Player.MovementAction.JumpExhausted")));

	FGameplayModifierInfo ResetJump;
	ResetJump.Attribute = UCrashAttributeSet::GetNumberOfJumpsAttribute();
	ResetJump.ModifierOp = EGameplayModOp::Override;
	ResetJump.ModifierMagnitude = FGameplayEffectModifierMagnitude(3.0f);

	Modifiers.Add((ResetJump));
}
