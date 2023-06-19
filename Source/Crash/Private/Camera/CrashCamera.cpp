// Fill out your copyright notice in the Description page of Project Settings.

#include "Camera/CrashCamera.h"
#include "Camera/CameraComponent.h"

// Sets default values
ACrashCamera::ACrashCamera()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	CameraComponent = CreateDefaultSubobject<UCameraComponent>("Camera"); //Create and attach camera component by default 
}

// Called when the game starts or when spawned
void ACrashCamera::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACrashCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

