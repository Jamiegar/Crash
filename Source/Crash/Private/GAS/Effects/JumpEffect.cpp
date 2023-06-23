// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Effects/JumpEffect.h"

#include "GAS/CrashAttributeSet.h"

UJumpEffect::UJumpEffect()
{
	DurationPolicy = EGameplayEffectDurationType::Instant;

	FGameplayModifierInfo JumpInfo;
	JumpInfo.Attribute = UCrashAttributeSet::GetNumberOfJumpsAttribute();
	JumpInfo.ModifierOp = EGameplayModOp::Additive;
	JumpInfo.ModifierMagnitude = FGameplayEffectModifierMagnitude(-1.0f);

	Modifiers.Add(JumpInfo);
}
