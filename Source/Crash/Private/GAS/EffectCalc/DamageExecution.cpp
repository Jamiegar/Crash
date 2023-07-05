// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/EffectCalc/DamageExecution.h"
#include "GAS/CrashAttributeSet.h"
#include "GAS/CrashGameplayEffectContext.h"
#include "GAS/CrashGameplayTags.h"

struct FDamageStats
{
	FGameplayEffectAttributeCaptureDefinition DamageDef;
	FGameplayEffectAttributeCaptureDefinition WeightDef;

	FDamageStats()
	{
		DamageDef = FGameplayEffectAttributeCaptureDefinition(UCrashAttributeSet::GetDamageAttribute(), EGameplayEffectAttributeCaptureSource::Target, false);
		WeightDef = FGameplayEffectAttributeCaptureDefinition(UCrashAttributeSet::GetWeightAttribute(), EGameplayEffectAttributeCaptureSource::Source, false);
	}
};

static const FDamageStats& DamageStats()
{
	static FDamageStats Stats;
	return Stats;
}


UDamageExecution::UDamageExecution()
{
	RelevantAttributesToCapture.Add(DamageStats().DamageDef);
	RelevantAttributesToCapture.Add(DamageStats().WeightDef);
}

void UDamageExecution::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	Super::Execute_Implementation(ExecutionParams, OutExecutionOutput);
	
	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
	
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluateParameters;
	EvaluateParameters.SourceTags = SourceTags;
	EvaluateParameters.TargetTags = TargetTags;

	const FCrashGameplayTags& GameTags = FCrashGameplayTags::Get();
	const float BaseDamage = FMath::Max<float>(Spec.GetSetByCallerMagnitude(GameTags.PlayerDamaged, false, -1.0f), 0.0f);

	float Weight = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStats().WeightDef, EvaluateParameters, Weight);

	const float DamageDone = BaseDamage * (1 - (Weight / 100));

	OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(UCrashAttributeSet::GetDamageAttribute(), EGameplayModOp::Additive, DamageDone));
}
