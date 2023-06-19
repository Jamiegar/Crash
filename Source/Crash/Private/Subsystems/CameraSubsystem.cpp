// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystems/CameraSubsystem.h"
#include "Kismet/GameplayStatics.h"

void UCameraSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

ACrashCamera* UCameraSubsystem::GetCamera()
{
	if(CameraActor)
		return CameraActor;

	AActor* Camera = UGameplayStatics::GetActorOfClass(GetWorld(), ACrashCamera::StaticClass());
	CameraActor = Cast<ACrashCamera>(Camera);

	if(CameraActor == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("No Crash Camera Actor in Scene"));
		return nullptr;
	}

	return CameraActor;
}
