// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CrashCharacter.h"
#include "CrashPlayerCharacter.generated.h"

class UInputMappingContext;
UCLASS()
class CRASH_API ACrashPlayerCharacter : public ACrashCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACrashPlayerCharacter();

protected:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category= "Input")
	TArray<UInputMappingContext*> DefaultInputMappings;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void SetupCameraView() const;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
