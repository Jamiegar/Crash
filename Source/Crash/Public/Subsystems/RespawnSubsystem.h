// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "RespawnSubsystem.generated.h"

class ACrashCharacter;
class AKillZone;
/**
 * 
 */
UCLASS()
class CRASH_API URespawnSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	virtual void OnWorldBeginPlay(UWorld& InWorld) override;
	void StartRespawnSequence(ACrashCharacter* Character) const;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="KillZone")
	AKillZone* KillZone;

	
};
