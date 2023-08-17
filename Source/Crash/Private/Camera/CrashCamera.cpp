// Fill out your copyright notice in the Description page of Project Settings.

#include "Camera/CrashCamera.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/BoxComponent.h"


// Sets default values
ACrashCamera::ACrashCamera()
{
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>("Camera Look At Target");
	SpringArm = CreateDefaultSubobject<USpringArmComponent>("Spring Arm");
	CameraComponent = CreateDefaultSubobject<UCameraComponent>("Camera"); //Create and attach camera component by default
	
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->bDoCollisionTest = false;
	SpringArm->bEnableCameraLag = true;
	SpringArm->CameraLagSpeed = 5.f;
	
	CameraComponent->SetupAttachment(SpringArm, USpringArmComponent::SocketName);

	CameraBounds = CreateDefaultSubobject<UBoxComponent>("Camera Bounds");
	ShakeComponent = CreateDefaultSubobject<UCameraShakeComponent>("Shake Component");
	
	SetActorTickEnabled(false);
}

void ACrashCamera::SetActiveCharacters(const TArray<const ACharacter*>& Characters)
{
	ActiveCharacters = Characters;
}

void ACrashCamera::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	CameraBounds->SetWorldLocation(Transform.GetLocation() + BoundsOffset);
	CameraBounds->SetWorldRotation(Transform.GetRotation());
	
	CameraBounds->SetBoxExtent(FVector(0, BoxExtent.X, BoxExtent.Y));
}

// Called when the game starts or when spawned
void ACrashCamera::BeginPlay()
{
	Super::BeginPlay();

	SetActorTickEnabled(true);
}

void ACrashCamera::GetMinMaxPosition(FVector& OutMax, FVector& OutMin)
{
	OutMin = CameraBounds->Bounds.Origin - CameraBounds->Bounds.BoxExtent;
	OutMax = CameraBounds->Bounds.Origin + CameraBounds->Bounds.BoxExtent;
}

FVector ACrashCamera::CalculateAverageLocation(TArray<const ACharacter*> Characters)
{
	FVector LocationSum(0,0,0); // sum of locations
	int32 ActorCount = 0; // num actors
	// iterate over actors
	for(int32 ActorIdx=0; ActorIdx<Characters.Num(); ActorIdx++)
	{
		const AActor* A = Characters[ActorIdx];
		// Check actor is non-null, not deleted, and has a root component
		if (IsValid(A) && A->GetRootComponent())
		{
			LocationSum += A->GetActorLocation();
			ActorCount++;
		}
	}

	// Find average
	FVector Average(0,0,0);
	if(ActorCount > 0)
	{
		Average = LocationSum/static_cast<float>(ActorCount);
	}
	return Average;
}

void ACrashCamera::CameraMovement(float DeltaTime)
{
	PreviousLocation = RootComponent->GetComponentLocation();
	const FVector AverageLocation = CalculateAverageLocation(ActiveCharacters);
	
	FVector NewLocation = FMath::VInterpTo(PreviousLocation, AverageLocation, DeltaTime, CameraMoveSpeed); //Interps the Look at component to the new location
	//NewLocation += CameraMovementOffset; //Adds an offset to the new location, as the character position is at the feet and so the average Location could be too low  
	
	FVector OutMin;
	FVector OutMax;
	GetMinMaxPosition(OutMax, OutMin); //Sets the input variables to the bounding box min and max

	//Clamp the new movement location within the bounding box
	NewLocation.X = FMath::Clamp(NewLocation.X, OutMin.X, OutMax.X);
	NewLocation.Z = FMath::Clamp(NewLocation.Z, OutMin.Z, OutMax.Z);
	
	//Set the location of the camera look At component
	RootComponent->SetWorldLocation(NewLocation);
}

void ACrashCamera::CameraZoom(float DeltaTime)
{
	float PreviousArmLength = SpringArm->TargetArmLength;
	TArray<float> DistancesBetweenChar;
	
	for (int CharIdx = 0; CharIdx < ActiveCharacters.Num(); CharIdx++)
	{
		const ACharacter* CurrentCharacter = ActiveCharacters[CharIdx];
		
		for(int i = CharIdx + 1; i < ActiveCharacters.Num(); i++)
		{
			const ACharacter* NextCharacter = ActiveCharacters[i];

			float Distance = CurrentCharacter->GetDistanceTo(NextCharacter);
			DistancesBetweenChar.Add(Distance);
		}
	}

	int IndexMaxValue;
	float FloatValue;
	UKismetMathLibrary::MaxOfFloatArray(DistancesBetweenChar, IndexMaxValue, FloatValue);

	FloatValue = FMath::Clamp(FloatValue, MinCameraDistance, MaxCameraDistance);

	const float NewArmLength = FMath::FInterpTo(PreviousArmLength, FloatValue, DeltaTime, ZoomSpeed);
	SpringArm->TargetArmLength = NewArmLength;
}

// Called every frame
void ACrashCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CameraMovement(DeltaTime);
	CameraZoom(DeltaTime);
}

