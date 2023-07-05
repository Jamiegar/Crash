// Fill out your copyright notice in the Description page of Project Settings.

#include "Characters/CrashPlayerCharacter.h"
#include "AbilitySystemComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Characters/CombatComponents/BasicCombatComponent.h"
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

void ACrashPlayerCharacter::InitializePlayerCharacter()
{
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

void ACrashPlayerCharacter::JumpInputActivate(const FInputActionValue& Value)
{
	//const FGameplayAbilitySpecHandle* AbilitySpecHandle = 
	//GetAbilitySystemComponent()->TryActivateAbility(*AbilitySpecHandle);
	//GetAbilitySystemComponent()->AbilityLocalInputPressed(EAbilityInputID::MovementJump);

	ActivateJumpAbility();
}

void ACrashPlayerCharacter::BasicMiddleAttackInput(const FInputActionValue& Value)
{
	BasicCombatComponent->BasicMiddleAttack();
}

void ACrashPlayerCharacter::BasicUpAttackInput(const FInputActionValue& Value)
{
	BasicCombatComponent->BasicUpAttack();
}

void ACrashPlayerCharacter::BasicDownAttackInput(const FInputActionValue& Value)
{
	BasicCombatComponent->BasicDownAttack();
}



// Called to bind functionality to input
void ACrashPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	if(UCrashEnhancedInputComponent* CrashInputComponent = CastChecked<UCrashEnhancedInputComponent>(PlayerInputComponent))
	{
		const FCrashGameplayTags& GameplayTags = FCrashGameplayTags::Get();
		
		CrashInputComponent->BindActionByTag(InputConfig, GameplayTags.Movement, ETriggerEvent::Triggered, this, &ACrashPlayerCharacter::Move);
		CrashInputComponent->BindActionByTag(InputConfig, GameplayTags.Jump, ETriggerEvent::Started, this, &ACrashPlayerCharacter::JumpInputActivate);
		CrashInputComponent->BindActionByTag(InputConfig, GameplayTags.LeftBasicAttack, ETriggerEvent::Completed, this, &ACrashPlayerCharacter::BasicMiddleAttackInput);
		CrashInputComponent->BindActionByTag(InputConfig, GameplayTags.RightBasicAttack, ETriggerEvent::Completed, this, &ACrashPlayerCharacter::BasicMiddleAttackInput);
		CrashInputComponent->BindActionByTag(InputConfig, GameplayTags.UpBasicAttack, ETriggerEvent::Completed, this, &ACrashPlayerCharacter::BasicUpAttackInput);
		CrashInputComponent->BindActionByTag(InputConfig, GameplayTags.DownBasicAttack, ETriggerEvent::Completed, this, &ACrashPlayerCharacter::BasicDownAttackInput);
	}
}

