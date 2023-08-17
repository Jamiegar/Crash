// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CameraShakeComponent.h"
#include "GameFramework/Actor.h"
#include "CrashCamera.generated.h"


class UCameraShakeComponent;
class UBoxComponent;
class UCameraComponent;
class USpringArmComponent;

UCLASS()
class CRASH_API ACrashCamera : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACrashCamera();

	void SetActiveCharacters(const TArray<const ACharacter*>& Characters);
	virtual void OnConstruction(const FTransform& Transform) override;

	UFUNCTION(BlueprintCallable, Category="Camera")
	UCameraComponent* GetCameraComponent() { return CameraComponent; }

	UFUNCTION(BlueprintCallable, Category="Camera Shake")
	UCameraShakeComponent* GetShakeComponent() { return ShakeComponent; }

protected:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Camera Movement")
	float CameraMoveSpeed = 3.5;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Camera Movement")
	float ZoomSpeed = 2.5;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Camera Movement")
	float MaxCameraDistance = 3000;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Camera Movement")
	float MinCameraDistance = 1000;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Camera Movement")
	FVector CameraMovementOffset = FVector(0, 0, 3.5);

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Camera Bounds")
	FVector BoundsOffset = FVector(0, 0, 500);
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Camera Bounds")
	FVector2f BoxExtent = FVector2f(2000, 500);
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Camera Bounds")
	UBoxComponent* CameraBounds;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	USpringArmComponent* SpringArm;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	UCameraComponent* CameraComponent;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Camera Shake")
	UCameraShakeComponent* ShakeComponent;

	UPROPERTY()
	TArray<const ACharacter*> ActiveCharacters;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	void GetMinMaxPosition(FVector& OutMax, FVector& OutMin);
	
	FVector CalculateAverageLocation(TArray<const ACharacter*> Characters);

	void CameraMovement(float DeltaTime);
	void CameraZoom(float DeltaTime);
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	FVector PreviousLocation;

};
