// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "StunAbilityData.generated.h"

/**
 * 
 */
UCLASS()
class CRASH_API UStunAbilityData : public UDataAsset
{
	GENERATED_BODY()

public:
	UStunAbilityData();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float StunDuration = 1.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float MovementUnits = 5.0f;
};
