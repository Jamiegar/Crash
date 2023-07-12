// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "CrashCharacter.h"
#include "InputAction.h"
#include "Characters/Input/AbilitySystemInputID.h"
#include "CrashPlayerCharacter.generated.h"


class UCrashEnhancedInputComponent;
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

	UPROPERTY()
	UCrashEnhancedInputComponent* CrashEnhancedInputComponent;
	
	void SetupCameraView() const;
	
	void Move(const FInputActionValue& Value);
	void JumpInputActivate(const FInputActionValue& Value);
	void BasicMiddleAttackInput(const FInputActionValue& Value);
	void BasicUpAttackInput(const FInputActionValue& Value);
	void BasicDownAttackInput(const FInputActionValue& Value);
	void BasicAttackInput(const FInputActionValue& Value);
	void BasicBlockInputPressed(const FInputActionValue& Value);
	void BasicBlockInputReleased(const FInputActionValue& Value);
	void SlideInputPressed(const FInputActionValue& Value);

	void SendLocalInputToAbilityComponent(const EAbilityInputID InputID, bool bWasPressed = true);
	
public:
	
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
};
