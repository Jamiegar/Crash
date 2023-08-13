// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "CrashGameInstance.generated.h"

class ACrashPlayerCharacter;
/**
 * 
 */
UCLASS()
class CRASH_API UCrashGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Character Selection")
	TMap<int, TSubclassOf<ACrashPlayerCharacter>> PlayerSelection;
};
