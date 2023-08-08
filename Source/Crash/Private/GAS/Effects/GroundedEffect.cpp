// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Effects/GroundedEffect.h"
#include "GAS/CrashAttributeSet.h"
#include "GAS/Effects/LandedJumpCooldownEffect.h"

UGroundedEffect::UGroundedEffect()
{
	DurationPolicy = EGameplayEffectDurationType::HasDuration;
	
	InheritableOwnedTagsContainer.AddTag(FGameplayTag::RequestGameplayTag(FName("Player.State.Grounded")));
	RemoveGameplayEffectsWithTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Player.State.Airborne")));
	RemoveGameplayEffectsWithTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Player.MovementAction.JumpExhausted")));
	RemoveGameplayEffectsWithTags.AddTag(FGameplayTag::RequestGameplayTag("Player.MovementAction.DashExhausted"));


	FConditionalGameplayEffect JumpCooldownEffect;
	JumpCooldownEffect.EffectClass = ULandedJumpCooldownEffect::StaticClass();
	ConditionalGameplayEffects.Add(JumpCooldownEffect);
	
	FGameplayModifierInfo ResetJump;
	ResetJump.Attribute = UCrashAttributeSet::GetNumberOfJumpsAttribute();
	ResetJump.ModifierOp = EGameplayModOp::Override;
	ResetJump.ModifierMagnitude = FGameplayEffectModifierMagnitude(2.0f);

	Modifiers.Add(ResetJump);
}


