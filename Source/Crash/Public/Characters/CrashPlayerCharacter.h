// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "CrashCharacter.h"
#include "InputAction.h"
#include "CrashPlayerCharacter.generated.h"


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

protected:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Input")
	TArray<FMappingContextData> DefaultInputMappings;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Input|Actions|Movement")
	UInputAction* MoveLeftRightAction;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Input|Actions|Movement")
	UInputAction* JumpAction;
	
	virtual void BeginPlay() override; // Called when the game starts or when spawned
	void SetupCameraView() const;
	
	void Move(const FInputActionValue& Value);
	void JumpActivate(const FInputActionValue& Value);
	void JumpCompleted(const FInputActionValue& Value);
	

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	


};
