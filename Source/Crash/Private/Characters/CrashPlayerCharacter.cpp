// Fill out your copyright notice in the Description page of Project Settings.

#include "Characters/CrashPlayerCharacter.h"
#include "EnhancedInputSubsystems.h"
#include "Subsystems/CameraSubsystem.h"


// Sets default values
ACrashPlayerCharacter::ACrashPlayerCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
}

// Called when the game starts or when spawned
void ACrashPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	SetupCameraView();
	
	if(const APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if(UEnhancedInputLocalPlayerSubsystem* LocalPlayerSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			for (const auto MappingContext : DefaultInputMappings)
				LocalPlayerSubsystem->AddMappingContext(MappingContext, 1);
		}
	}
}

void ACrashPlayerCharacter::SetupCameraView() const
{
	if(UCameraSubsystem* CameraSubsystem = GetWorld()->GetSubsystem<UCameraSubsystem>())
	{
		if(APlayerController* PlayerController = Cast<APlayerController>(GetController()))
		{
			PlayerController->SetViewTarget(CameraSubsystem->GetCamera());
		}	
	}
}

// Called every frame
void ACrashPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ACrashPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

