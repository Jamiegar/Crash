// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "InputConfig.generated.h"

class UInputAction;

USTRUCT(BlueprintType)
struct FTaggedInputAction
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly)
	const UInputAction* InputAction = nullptr;

	UPROPERTY(EditDefaultsOnly, meta=(Categories = "InputTag"))
	FGameplayTag InputTag;
};

UCLASS()
class CRASH_API UInputConfig : public UDataAsset
{
	GENERATED_BODY()
public:
	const UInputAction* FindInputActionForTag(const FGameplayTag& InputTag) const;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(TitleProperty = "InputAction"))
	TArray<FTaggedInputAction> TaggedInputActions;
	
};
