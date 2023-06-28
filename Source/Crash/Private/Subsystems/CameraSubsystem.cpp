// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystems/CameraSubsystem.h"
#include "Kismet/GameplayStatics.h"

void UCameraSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

ACrashCamera* UCameraSubsystem::GetCamera()
{
	if(CameraActor) //If camera is valid return ptr to Camera
		return CameraActor; //Return ptr to camera

	//If the camera == nullptr find the camera in the scene and save variable to it
	AActor* Camera = UGameplayStatics::GetActorOfClass(GetWorld(), ACrashCamera::StaticClass());
	CameraActor = Cast<ACrashCamera>(Camera);

	if(CameraActor == nullptr) //Log to console there is no camera in the scene
	{
		UE_LOG(LogTemp, Error, TEXT("No Crash Camera Actor in Scene"));
		return nullptr;
	}
	return CameraActor; //Return ptr to camera
}
