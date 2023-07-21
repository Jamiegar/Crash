// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "KnockbackData.generated.h"


UCLASS()
class CRASH_API UKnockbackData : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float UpwardForce = 100;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float HorizontalForce = 100;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float KnockbackGroundedDuration = 0.5;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bShouldCorkscrew = true;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UCurveFloat* ScrewCurve;
};
