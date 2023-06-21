// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "CrashGameplayAbility.generated.h"


UENUM()
enum EAbilityInputID
{
	None = 0,
	MovementJump,
	BasicAttackLeft,
	BasicAttackRight,
	BasicAttackUp,
	BasicAttackDown
};

UCLASS()
class CRASH_API UCrashGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UCrashGameplayAbility();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input")
	TEnumAsByte<EAbilityInputID> AbilityInputID = None;
	
	
	
	
	
};
