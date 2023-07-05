// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/EffectCalc/KnockbackExecution.h"
#include "GAS/CrashAttributeSet.h"
#include "GAS/CrashGameplayEffectContext.h"
#include "GAS/CrashGameplayTags.h"

struct FKnockbackStats
{
	
	FGameplayEffectAttributeCaptureDefinition KnockbackDef;
	FGameplayEffectAttributeCaptureDefinition WeightDef;
	FGameplayEffectAttributeCaptureDefinition PercentageDef;
	
	FKnockbackStats()
	{
		KnockbackDef = FGameplayEffectAttributeCaptureDefinition(UCrashAttributeSet::GetKnockbackAttribute(), EGameplayEffectAttributeCaptureSource::Target, false);
		WeightDef = FGameplayEffectAttributeCaptureDefinition(UCrashAttributeSet::GetWeightAttribute(), EGameplayEffectAttributeCaptureSource::Target, false);
		PercentageDef = FGameplayEffectAttributeCaptureDefinition(UCrashAttributeSet::GetPercentageAttribute(), EGameplayEffectAttributeCaptureSource::Target, false);
	}
};

static FKnockbackStats& KnockbackStats()
{
	static FKnockbackStats StatsKnockback;
	return StatsKnockback;
}

UKnockbackExecution::UKnockbackExecution()
{
	RelevantAttributesToCapture.Add(KnockbackStats().KnockbackDef);
	RelevantAttributesToCapture.Add(KnockbackStats().WeightDef);
	RelevantAttributesToCapture.Add(KnockbackStats().PercentageDef);
}

void UKnockbackExecution::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	Super::Execute_Implementation(ExecutionParams, OutExecutionOutput);

	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluateParameters;
	EvaluateParameters.SourceTags = SourceTags;
	EvaluateParameters.TargetTags = TargetTags;
	
	const FCrashGameplayEffectContext* TypeContext = FCrashGameplayEffectContext::ExtractEffectContext(Spec.GetContext());
	check(TypeContext);
	
	const float KnockBackScalar = FMath::Max<float>(Spec.GetSetByCallerMagnitude("Player.Damaged.Knockback", false, 0.0f), 0.0f);

	float Percent = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(KnockbackStats().PercentageDef, EvaluateParameters, Percent);
	
	float Weight = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(KnockbackStats().WeightDef, EvaluateParameters, Weight);
	
	const float KnockbackMag = Percent * (KnockBackScalar * (100 / Weight + 5));

	UE_LOG(LogTemp, Warning, TEXT("Knockback Magnitude: %f"), KnockbackMag);
	OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(UCrashAttributeSet::GetKnockbackAttribute(), EGameplayModOp::Override, KnockbackMag));
}
