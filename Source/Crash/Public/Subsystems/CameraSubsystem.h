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
	
	UPROPERTY()
	TArray<const ACharacter*> RegisteredCharacters;
	
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	UFUNCTION()
	virtual void OnGameSetUpComplete();


public:
	UFUNCTION(BlueprintPure, BlueprintInternalUseOnly)
	ACrashCamera* GetCamera();

	UFUNCTION(BlueprintCallable)
	void RegisterPlayerToCamera(const ACharacter* Character);
	
	TArray<const ACharacter*>& GetRegisteredCharacters() { return RegisteredCharacters; }

	UFUNCTION(BlueprintCallable, Category="Camera Shake")
	void ApplyCameraShake(float Trauma, float Decay = 1.3f, float TraumaMultiplier = 5, float TraumaMagnitude = 10.0f, float TraumaRotationalMagnitude = 0.7f, float TraumaDepthMagnitude = 0.0f);
};
