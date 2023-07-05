﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "CrashCharacter.h"
#include "InputAction.h"
#include "CrashPlayerCharacter.generated.h"


class UInputConfig;
class UInputMappingContext;

USTRUCT(DisplayName="Default Mapping Context Data", BlueprintType)
struct FMappingContextData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	UInputMappingContext* MappingContext;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	int Priority = 0;
};

UCLASS()
class CRASH_API ACrashPlayerCharacter : public ACrashCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACrashPlayerCharacter();

	void InitializePlayerCharacter();

protected:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Input")
	TArray<FMappingContextData> DefaultInputMappings;
	
	UPROPERTY(EditDefaultsOnly, Category="Input")
	UInputConfig* InputConfig;
	
	void SetupCameraView() const;
	
	void Move(const FInputActionValue& Value);
	void JumpInputActivate(const FInputActionValue& Value);
	void BasicMiddleAttackInput(const FInputActionValue& Value);
	void BasicUpAttackInput(const FInputActionValue& Value);
	void BasicDownAttackInput(const FInputActionValue& Value);
	
public:
	
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
};
