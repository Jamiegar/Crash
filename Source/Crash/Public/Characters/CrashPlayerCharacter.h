// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "CrashCharacter.h"
#include "InputAction.h"
#include "Characters/Input/AbilitySystemInputID.h"
#include "CrashPlayerCharacter.generated.h"

class UComboBasic;
class UInputAbilityMap;
class UCrashEnhancedInputComponent;
class UInputMappingContext;

USTRUCT(Blueprintable)
struct CRASH_API FAbilityInputMap
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	TMap<UInputAction*, TSubclassOf<UCrashGameplayAbility>> AbilityInputMappingLayout;

	void LoadDefaults();
};


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

	UFUNCTION(BlueprintCallable, BlueprintGetter)
	FAbilityInputMap& GetInputAbilityMap() { return InputAbilityMap; }

	virtual void PossessedBy(AController* NewController) override;

	
protected:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Input")
	TArray<FMappingContextData> DefaultInputMappings;

	UPROPERTY(EditDefaultsOnly, Category="Input")
	UInputAction* MovementAction;
	
	UPROPERTY(EditDefaultsOnly, Category="Input")
	FAbilityInputMap InputAbilityMap;

	UPROPERTY()
	UCrashEnhancedInputComponent* CrashEnhancedInputComponent;

	void GiveInputMapAbilitiesToCharacter();
	void SetupCameraView();
	
	void Move(const FInputActionValue& Value);
	
	UFUNCTION()
	void OnAbilityInputPressed(const FInputActionValue& ActionValue, float ElapsedTime, float TriggeredTime, const UInputAction* SourceAction);

	UFUNCTION()
	void OnAbilityInputReleased(const FInputActionValue& ActionValue, float ElapsedTime, float TriggeredTime, const UInputAction* SourceAction);

	void BindAbiltiesToInput(UCrashEnhancedInputComponent* CrashInputComponent);
	void SendLocalInputToAbilityComponent(const EAbilityInputID InputID, bool bWasPressed = true);

public:
	
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:

	UPROPERTY()
	FVector ConstantCharacterForward = FVector();
};
