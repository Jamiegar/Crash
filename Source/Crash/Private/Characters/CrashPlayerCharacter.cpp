﻿// Fill out your copyright notice in the Description page of Project Settings.

#include "Characters/CrashPlayerCharacter.h"
#include "AbilitySystemComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Characters/Input/CrashEnhancedInputComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Subsystems/CameraSubsystem.h"
#include "InputMappingContext.h"
#include "Characters/Input/InputAbilityMap.h"
#include "Components/WidgetComponent.h"
#include "Crash/CrashGameModeBase.h"
#include "GAS/Abiliities/Combat/Basic/BlockAbility.h"
#include "GAS/Abiliities/Combat/Basic/ComboBasic.h"
#include "GAS/Abiliities/Combat/Basic/DownBasic.h"
#include "GAS/Abiliities/Combat/Basic/MiddleBasic.h"
#include "GAS/Abiliities/Combat/Basic/SlideAbility.h"
#include "GAS/Abiliities/Combat/Basic/UpBasic.h"
#include "GAS/Abiliities/Movement/JumpAbility.h"
#include "GAS/Abiliities/Movement/PhasePlatform.h"
#include "InterfacesUI/PlayerUILocator.h"
#include "Kismet/GameplayStatics.h"


#define LOCTEXT_NAMESPACE "FAbilityInputMap"
void FAbilityInputMap::LoadDefaults()
{
	// --- Find Default Input Actions --------------------------------
	UInputAction* BasicNeutralInput = LoadObject<UInputAction>(nullptr, TEXT("/Game/Blueprints/Characters/Input/CombatInputActions/IA_BasicAttack.IA_BasicAttack"));
	UInputAction* BasicUpInput = LoadObject<UInputAction>(nullptr, TEXT("/Game/Blueprints/Characters/Input/CombatInputActions/IA_BasicUpAttack.IA_BasicUpAttack"));
	UInputAction* BasicLeftInput = LoadObject<UInputAction>(nullptr, TEXT("/Game/Blueprints/Characters/Input/CombatInputActions/IA_BasicLeftAttack.IA_BasicLeftAttack"));
	UInputAction* BasicRightInput = LoadObject<UInputAction>(nullptr, TEXT("/Game/Blueprints/Characters/Input/CombatInputActions/IA_BasicRightAttack.IA_BasicRightAttack"));
	UInputAction* BasicDownInput = LoadObject<UInputAction>(nullptr, TEXT("/Game/Blueprints/Characters/Input/CombatInputActions/IA_BasicDownAttack.IA_BasicDownAttack"));
	UInputAction* BlockInput = LoadObject<UInputAction>(nullptr, TEXT("/Game/Blueprints/Characters/Input/CombatInputActions/IA_Block.IA_Block"));
	UInputAction* SlideInput = LoadObject<UInputAction>(nullptr, TEXT("/Game/Blueprints/Characters/Input/CombatInputActions/IA_Slide.IA_Slide"));
	UInputAction* JumpInput = LoadObject<UInputAction>(nullptr, TEXT("/Game/Blueprints/Characters/Input/MovementInput/IA_Jump.IA_Jump"));
	UInputAction* DownMovementInput = LoadObject<UInputAction>(nullptr, TEXT("/Game/Blueprints/Characters/Input/MovementInput/IA_DownMovement.IA_DownMovement"));
	

	// --- Find Default Abilities ------------------------------------
	const UComboBasic* ComboBasic = LoadObject<UComboBasic>(nullptr, TEXT("/Script/CoreUObject.Class'/Script/Crash.ComboBasic'"));
	const UUpBasic* BasicUpAttack = LoadObject<UUpBasic>(nullptr, TEXT("/Script/CoreUObject.Class'/Script/Crash.UpBasic'"));
	const UMiddleBasic* BasicMiddleAttack = LoadObject<UMiddleBasic>(nullptr, TEXT("/Script/CoreUObject.Class'/Script/Crash.MiddleBasic'"));
	const UDownBasic* BasicDownAttack = LoadObject<UDownBasic>(nullptr, TEXT("/Script/CoreUObject.Class'/Script/Crash.DownBasic'"));
	const UBlockAbility* BlockAbility = LoadObject<UBlockAbility>(nullptr, TEXT("/Script/CoreUObject.Class'/Script/Crash.BlockAbility'"));
	const USlideAbility* SlideAbility = LoadObject<USlideAbility>(nullptr, TEXT("/Script/CoreUObject.Class'/Script/Crash.SlideAbility'"));
	const UJumpAbility* JumpAbility = LoadObject<UJumpAbility>(nullptr, TEXT("/Script/CoreUObject.Class'/Script/Crash.JumpAbility'"));
	const UPhasePlatform* PhasePlatformAbility = LoadObject<UPhasePlatform>(nullptr, TEXT("/Script/Crash.PhasePlatform"));

	
	// --- Default Input Ability Actions ------------------------------
	AbilityInputMappingLayout.Add(BasicLeftInput, BasicMiddleAttack->StaticClass());
	AbilityInputMappingLayout.Add(BasicRightInput, BasicMiddleAttack->StaticClass());
	AbilityInputMappingLayout.Add(BlockInput, BlockAbility->StaticClass());
	AbilityInputMappingLayout.Add(JumpInput, JumpAbility->StaticClass());
	AbilityInputMappingLayout.Add(BasicUpInput, BasicUpAttack->StaticClass());
	AbilityInputMappingLayout.Add(BasicDownInput, BasicDownAttack->StaticClass());
	AbilityInputMappingLayout.Add(SlideInput, SlideAbility->StaticClass());
	AbilityInputMappingLayout.Add(BasicNeutralInput, ComboBasic->StaticClass());
	AbilityInputMappingLayout.Add(DownMovementInput, PhasePlatformAbility->StaticClass());
}
#undef LOCTEXT_NAMESPACE

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

	static ConstructorHelpers::FObjectFinder<UInputAction> DefaultMovementAction
		(TEXT("/Script/EnhancedInput.InputAction'/Game/Blueprints/Characters/Input/MovementInput/IA_MovementLeftRight.IA_MovementLeftRight'"));

	MovementAction = DefaultMovementAction.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction> DefaultPauseAction
		(TEXT("/Script/EnhancedInput.InputAction'/Game/Blueprints/Characters/Input/IA_Pause.IA_Pause'"));

	PauseAction = DefaultPauseAction.Object;
	
	InputAbilityMap.LoadDefaults();
	GiveInputMapAbilitiesToCharacter();
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
		
		if(const UUserWidget* LocatorWidget = PlayerLocatorWidget->GetWidget())
		{
			if(LocatorWidget->Implements<UPlayerUILocator>())
			{
				IPlayerUILocator::Execute_SetPlayerID(PlayerLocatorWidget->GetWidget(), PlayerController->GetLocalPlayer()->GetControllerId());
			}
		}
	}
}

void ACrashPlayerCharacter::PossessedBy(AController* NewController)
{
	GiveInputMapAbilitiesToCharacter();
	Super::PossessedBy(NewController);
}

void ACrashPlayerCharacter::GiveInputMapAbilitiesToCharacter()
{
	TArray<TSubclassOf<UCrashGameplayAbility>> AbilityArray;
	InputAbilityMap.AbilityInputMappingLayout.GenerateValueArray(AbilityArray);

	for (auto Ability : AbilityArray)
	{
		if(DefaultAbilities.Contains(Ability))
			continue;

		DefaultAbilities.Add(Ability);
	}
}

void ACrashPlayerCharacter::SetupCameraView()
{
	if(UCameraSubsystem* CameraSubsystem = GetWorld()->GetSubsystem<UCameraSubsystem>())
	{
		CameraSubsystem->RegisterPlayerToCamera(this);
		ConstantCharacterForward = UKismetMathLibrary::GetRightVector(CameraSubsystem->GetCamera()->GetActorRotation());
	}
	
}

void ACrashPlayerCharacter::Move(const FInputActionValue& Value)
{
	if(!bCanMove)
		return;
	
	const float AxisValue = Value.Get<float>();
	AddMovementInput(ConstantCharacterForward, AxisValue, true);
}

void ACrashPlayerCharacter::OnAbilityInputPressed(const FInputActionValue& ActionValue, float ElapsedTime, float TriggeredTime, const UInputAction* SourceAction)
{
	const TSubclassOf<UCrashGameplayAbility>* AbilityMap = InputAbilityMap.AbilityInputMappingLayout.Find(SourceAction);
	const EAbilityInputID InputID = AbilityMap->Get()->GetDefaultObject<UCrashGameplayAbility>()->AbilityInputID;
	SendLocalInputToAbilityComponent(InputID);
}

void ACrashPlayerCharacter::OnAbilityInputReleased(const FInputActionValue& ActionValue, float ElapsedTime, float TriggeredTime, const UInputAction* SourceAction)
{
	const TSubclassOf<UCrashGameplayAbility>* AbilityMap = InputAbilityMap.AbilityInputMappingLayout.Find(SourceAction);
	const EAbilityInputID InputID = AbilityMap->Get()->GetDefaultObject<UCrashGameplayAbility>()->AbilityInputID;
	SendLocalInputToAbilityComponent(InputID, false);
	
}

void ACrashPlayerCharacter::OnPausePressed()
{
	ACrashGameModeBase* GameMode = Cast<ACrashGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	if(UGameplayStatics::IsGamePaused(GetWorld()))
	{
		//UnPause
		GameMode->SetGamePause(false);
	}
	else
	{
		//Pause
		GameMode->SetGamePause(true);
	}
}

void ACrashPlayerCharacter::BindAbiltiesToInput(UCrashEnhancedInputComponent* CrashInputComponent)
{
	TArray<UInputAction*> AbilityInputActions;
	InputAbilityMap.AbilityInputMappingLayout.GetKeys(AbilityInputActions);
	
	for (const UInputAction* Action : AbilityInputActions)
	{
		CrashInputComponent->BindAction(Action, ETriggerEvent::Started, this, "OnAbilityInputPressed");
		CrashInputComponent->BindAction(Action, ETriggerEvent::Completed, this, "OnAbilityInputReleased");
		CrashInputComponent->BindAction(Action, ETriggerEvent::Canceled, this, "OnAbilityInputReleased");
	}
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
		CrashInputComponent->BindAction(MovementAction, ETriggerEvent::Triggered, this, &ACrashPlayerCharacter::Move);
		CrashInputComponent->BindAction(PauseAction, ETriggerEvent::Started, this, &ACrashPlayerCharacter::OnPausePressed);
		BindAbiltiesToInput(CrashInputComponent);
	}
}

