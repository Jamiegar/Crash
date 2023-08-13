// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PlayerUILocator.generated.h"

// This class does not need to be modified.
UINTERFACE(Blueprintable)
class UPlayerUILocator : public UInterface
{
	GENERATED_BODY()
};


class CRASH_API IPlayerUILocator
{
	GENERATED_BODY()

	
public:
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void SetPlayerID(int ID);
};
