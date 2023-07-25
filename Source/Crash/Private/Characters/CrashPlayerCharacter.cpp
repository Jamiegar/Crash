// Fill out your copyright notice in the Description page of Project Settings.

#include "Characters/CrashPlayerCharacter.h"
#include "AbilitySystemComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Characters/Input/CrashEnhancedInputComponent.h"
#include "GAS/CrashGameplayTags.h"
#include "Kismet/KismetMathLibrary.h"
#include "Subsystems/CameraSubsystem.h"
#include "InputMappingContext.h"
#include "Characters/Input/InputConfig.h"




// Sets default values
ACrashPlayerCharacter::ACrashPlayerCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UInputMappingContext> MovementMappingContext
		(TEXT("/Script/EnhancedInput.InputMappingContext'/Game/Blueprints/Characters/Input/IMC_CrashMovement.IMC_CrashMovement'"));

	static ConstructorHelpers::FObjectFinder<UInputMappingContext> CombatMappingContext
		(TEXT("/Script/EnhancedInput.InputMappingContext'/Game/Blueprints/Characters/Input/IMC_CrashCombat.IMC_CrashCombat'"));

	FMappingContextData MovementData;
	MovementData.MappingContext = MovementMappingContext.Object;

	FMappingContextData CombatData;
	CombatData.MappingContext = CombatMappingContext.Object;
	
	DefaultInputMappings.Add(MovementData);
	DefaultInputMappings.Add(CombatData);

	static ConstructorHelpers::FObjectFinder<UInputConfig> DefaultInputConfig
		(TEXT("/Script/Crash.InputConfig'/Game/Blueprints/Characters/Input/DA_CharacterInputConfig.DA_CharacterInputConfig'"));

	InputConfig = DefaultInputConfig.Object;
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
		CameraSubsystem->RegisterPlayerToCamera(this);
}

void ACrashPlayerCharacter::Move(const FInputActionValue& Value)
{
	const float AxisValue = Value.Get<float>();

	const FVector ForwardDirection = UKismetMathLibrary::GetForwardVector(FRotator(0, GetControlRotation().Yaw, 0));
	AddMovementInput(ForwardDirection, AxisValue, true);
}

void ACrashPlayerCharacter::JumpInputActivate(const FInputActionValue& Value)
{
	SendLocalInputToAbilityComponent(EAbilityInputID::MovementJump);
}

void ACrashPlayerCharacter::BasicMiddleAttackInput(const FInputActionValue& Value)
{
	SendLocalInputToAbilityComponent(EAbilityInputID::BasicAttackMiddle);
}

void ACrashPlayerCharacter::BasicUpAttackInput(const FInputActionValue& Value)
{
	SendLocalInputToAbilityComponent(EAbilityInputID::BasicAttackUp);
}

void ACrashPlayerCharacter::BasicDownAttackInput(const FInputActionValue& Value)
{
	SendLocalInputToAbilityComponent(EAbilityInputID::BasicAttackDown);
}

void ACrashPlayerCharacter::BasicAttackInput(const FInputActionValue& Value)
{
	SendLocalInputToAbilityComponent(EAbilityInputID::BasicAttack);
}

void ACrashPlayerCharacter::BasicBlockInputPressed(const FInputActionValue& Value)
{
	SendLocalInputToAbilityComponent(EAbilityInputID::Block);
}

void ACrashPlayerCharacter::BasicBlockInputReleased(const FInputActionValue& Value)
{
	SendLocalInputToAbilityComponent(EAbilityInputID::Block, false);
}

void ACrashPlayerCharacter::SlideInputPressed(const FInputActionValue& Value)
{
	SendLocalInputToAbilityComponent(EAbilityInputID::Slide);
}

void ACrashPlayerCharacter::SendLocalInputToAbilityComponent(const EAbilityInputID InputID, bool bWasPressed)
{
	check(AbilityComponent);

	if(bWasPressed)
		AbilityComponent->AbilityLocalInputPressed(static_cast<int32>(InputID));
	else
		AbilityComponent->AbilityLocalInputReleased(static_cast<int32>(InputID));
}

// Called to bind functionality to input
void ACrashPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	CrashEnhancedInputComponent = CastChecked<UCrashEnhancedInputComponent>(PlayerInputComponent);
	
	if(UCrashEnhancedInputComponent* CrashInputComponent = CastChecked<UCrashEnhancedInputComponent>(PlayerInputComponent))
	{
		const FCrashGameplayTags& GameplayTags = FCrashGameplayTags::Get();

		
		CrashInputComponent->BindActionByTag(InputConfig, GameplayTags.Movement, ETriggerEvent::Triggered, this, &ACrashPlayerCharacter::Move);
		CrashInputComponent->BindActionByTag(InputConfig, GameplayTags.Jump, ETriggerEvent::Started, this, &ACrashPlayerCharacter::JumpInputActivate);
		CrashInputComponent->BindActionByTag(InputConfig, GameplayTags.LeftBasicAttack, ETriggerEvent::Started, this, &ACrashPlayerCharacter::BasicMiddleAttackInput);
		CrashInputComponent->BindActionByTag(InputConfig, GameplayTags.RightBasicAttack, ETriggerEvent::Started, this, &ACrashPlayerCharacter::BasicMiddleAttackInput);
		CrashInputComponent->BindActionByTag(InputConfig, GameplayTags.UpBasicAttack, ETriggerEvent::Started, this, &ACrashPlayerCharacter::BasicUpAttackInput);
		CrashInputComponent->BindActionByTag(InputConfig, GameplayTags.DownBasicAttack, ETriggerEvent::Started, this, &ACrashPlayerCharacter::BasicDownAttackInput);
		CrashInputComponent->BindActionByTag(InputConfig, GameplayTags.BasicAttack, ETriggerEvent::Started, this, &ACrashPlayerCharacter::BasicAttackInput);
		CrashInputComponent->BindActionByTag(InputConfig, GameplayTags.Block, ETriggerEvent::Started, this, &ACrashPlayerCharacter::BasicBlockInputPressed);
		CrashInputComponent->BindActionByTag(InputConfig, GameplayTags.Block, ETriggerEvent::Completed, this, &ACrashPlayerCharacter::BasicBlockInputReleased);
		CrashInputComponent->BindActionByTag(InputConfig, GameplayTags.Slide, ETriggerEvent::Started, this, &ACrashPlayerCharacter::SlideInputPressed);
	}
}

