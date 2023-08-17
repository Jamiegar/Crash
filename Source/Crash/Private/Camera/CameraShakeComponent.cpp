// Fill out your copyright notice in the Description page of Project Settings.


#include "Camera/CameraShakeComponent.h"
#include "Camera/CameraComponent.h"
#include "Camera/CrashCamera.h"
#include "Kismet/KismetMathLibrary.h"


// Sets default values for this component's properties
UCameraShakeComponent::UCameraShakeComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

void UCameraShakeComponent::SetShakeActive(bool NewActive)
{
	IsShakeActive = NewActive;

	if(IsShakeActive)
	{
		SetComponentTickEnabled(true);
	}
	else
	{
		SetComponentTickEnabled(false);
	}
}

void UCameraShakeComponent::BeginPlay()
{
	Super::BeginPlay();

	if(ACrashCamera* CrashCamera = Cast<ACrashCamera>(GetOwner()))
	{
		CameraComponent = CrashCamera->GetCameraComponent();
	}
	
	check(CameraComponent);
	
	SetComponentTickEnabled(true);
}

void UCameraShakeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if(!IsShakeActive)
		return;
	

	if(Trauma > 0)
	{
		TimeCounter += DeltaTime * FMath::Cube(Trauma) * TraumaMultiplier;

		const FVector NewCameraPos = GetNoiseVector() * TraumaMagnitude *  Trauma;
		CameraComponent->SetRelativeLocation(NewCameraPos);

		const FRotator NewCameraRot = GetNoiseRotator() * TraumaRotationalMagnitude; 
		CameraComponent->SetRelativeRotation(NewCameraRot);

		SetTrauma(Trauma - (DeltaTime * Decay));
	}
	else
	{
		FVector LerpDefaultPos = UKismetMathLibrary::VInterpTo_Constant(CameraComponent->GetRelativeLocation(), FVector::Zero(), DeltaTime, 25);
		CameraComponent->SetRelativeLocation(LerpDefaultPos);

		FRotator LerpDefaultRot = UKismetMathLibrary::RInterpTo(CameraComponent->GetRelativeRotation(), FRotator::ZeroRotator, DeltaTime, 10);
		CameraComponent->SetRelativeRotation(LerpDefaultRot);
	}
	
}

float UCameraShakeComponent::GetPerlinNoiseValue(float Seed) const
{
	return FMath::PerlinNoise2D(FVector2D(Seed, TimeCounter));
}

FVector UCameraShakeComponent::GetNoiseVector()
{
	float X = GetPerlinNoiseValue(10) * TraumaDepthMagnitude;
	float Y = GetPerlinNoiseValue(20);
	float Z = GetPerlinNoiseValue(30);
	
	return FVector(X, Y, Z);
}

FRotator UCameraShakeComponent::GetNoiseRotator()
{
	float Pitch = GetPerlinNoiseValue(40);
	float Yaw = GetPerlinNoiseValue(50);
	float Roll = GetPerlinNoiseValue(60);

	return FRotator(Pitch, Yaw, Roll);
}

