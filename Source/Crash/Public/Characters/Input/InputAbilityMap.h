// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "Engine/DataAsset.h"
#include "InputAbilityMap.generated.h"


class UCrashGameplayAbility;
class UInputAction;

UCLASS()
class CRASH_API UInputAbilityMap : public UDataAsset
{
	GENERATED_BODY()

public:
	UInputAbilityMap();

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	TMap<UInputAction*, TSubclassOf<UCrashGameplayAbility>> AbilityInputMappingLayout;
};
