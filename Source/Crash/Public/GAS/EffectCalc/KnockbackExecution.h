// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "KnockbackExecution.generated.h"

/**
 * 
 */
UCLASS()
class CRASH_API UKnockbackExecution : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()

public:
	UKnockbackExecution();
	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
};
