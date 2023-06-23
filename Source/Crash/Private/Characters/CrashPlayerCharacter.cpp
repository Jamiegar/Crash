// Fill out your copyright notice in the Description page of Project Settings.

#include "Characters/CrashPlayerCharacter.h"
#include "AbilitySystemComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Characters/Input/CrashEnhancedInputComponent.h"
#include "GAS/CrashGameplayTags.h"
#include "GAS/Abiliities/Movement/JumpAbility.h"
#include "Kismet/KismetMathLibrary.h"
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

	// Add input mapping context to player 
	if(const APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if(UEnhancedInputLocalPlayerSubsystem* LocalPlayerSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			for (const auto ContextData : DefaultInputMappings)
			{
				LocalPlayerSubsystem->AddMappingContext(ContextData.MappingContext, ContextData.Priority);
			}
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

void ACrashPlayerCharacter::Move(const FInputActionValue& Value)
{
	const float AxisValue = Value.Get<float>();

	const FVector ForwardDirection = UKismetMathLibrary::GetForwardVector(FRotator(0, GetControlRotation().Yaw, 0));
	AddMovementInput(ForwardDirection, AxisValue, true);
}

void ACrashPlayerCharacter::JumpActivate(const FInputActionValue& Value)
{
	//const FGameplayAbilitySpecHandle* AbilitySpecHandle = AbilityMap.Find(UJumpAbility::StaticClass());
	//GetAbilitySystemComponent()->TryActivateAbility(*AbilitySpecHandle);

	GetAbilitySystemComponent()->AbilityLocalInputPressed(EAbilityInputID::MovementJump);
	
}

void ACrashPlayerCharacter::JumpCompleted(const FInputActionValue& Value)
{
	if(GetAbilitySystemComponent()->HasActivatableTriggeredAbility(FGameplayTag::RequestGameplayTag(FName("Player.MovementAction.Jump"))))
		GetAbilitySystemComponent()->AbilityLocalInputReleased(EAbilityInputID::MovementJump);
}


// Called to bind functionality to input
void ACrashPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	if(UCrashEnhancedInputComponent* CrashInputComponent = CastChecked<UCrashEnhancedInputComponent>(PlayerInputComponent))
	{
		const FCrashGameplayTags& GameplayTags = FCrashGameplayTags::Get();
		
		CrashInputComponent->BindActionByTag(InputConfig, GameplayTags.Movement, ETriggerEvent::Triggered, this, &ACrashPlayerCharacter::Move);
		CrashInputComponent->BindActionByTag(InputConfig, GameplayTags.Jump, ETriggerEvent::Started, this, &ACrashPlayerCharacter::JumpActivate);
		CrashInputComponent->BindActionByTag(InputConfig, GameplayTags.Jump, ETriggerEvent::Completed, this, &ACrashPlayerCharacter::JumpCompleted);
	}
}

