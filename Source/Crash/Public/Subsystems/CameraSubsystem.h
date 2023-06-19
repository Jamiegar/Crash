// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "Camera/CrashCamera.h"
#include "CameraSubsystem.generated.h"

/**
 * 
 */

UCLASS()
class CRASH_API UCameraSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

private:
	UPROPERTY(BlueprintGetter=GetCamera)
	ACrashCamera* CameraActor = nullptr;
	
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

public:
	UFUNCTION(BlueprintPure, BlueprintInternalUseOnly)
	ACrashCamera* GetCamera();
	
};
