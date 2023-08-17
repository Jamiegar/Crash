// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/CrashCharacter.h"
#include "GameplayTagsManager.h"
#include "Characters/CombatComponents/CombatComponent.h"
#include "Characters/CombatComponents/KnockbackComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GAS/Abiliities/Combat/Basic/BlockAbility.h"
#include "GAS/Abiliities/Combat/Basic/SlideAbility.h"
#include "GAS/Abiliities/Movement/JumpAbility.h"
#include "GAS/Effects/AirborneEffect.h"
#include "GAS/Effects/GroundedEffect.h"
#include "Components/TimelineComponent.h"
#include "Components/WidgetComponent.h"
#include "GAS/CrashAbilitySystemComponent.h"
#include "GAS/Abiliities/Combat/AirAttackAbility.h"
#include "GAS/Abiliities/Combat/Damage/DeathEffect.h"
#include "GAS/Abiliities/Combat/Damage/RespawnAbility.h"
#include "GAS/Effects/Damaging/DamageLivesInstant.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Subsystems/CameraSubsystem.h"


// Sets default values
ACrashCharacter::ACrashCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	ACrashCharacter::SetUpDefaultMovementValues();
	
	AbilityComponent = CreateDefaultSubobject<UCrashAbilitySystemComponent>("Ability Component");
	CombatComponent = CreateDefaultSubobject<UCombatComponent>("Basic Combat Component");
	KnockbackComponent = CreateDefaultSubobject<UKnockbackComponent>("Knockback Component");

	MeshAttachment = CreateDefaultSubobject<USceneComponent>("Mesh Attachment");
	MeshAttachment->SetupAttachment(RootComponent);
	GetMesh()->SetupAttachment(MeshAttachment, MeshAttachment->GetAttachSocketName());
	
	PlayerLocatorWidget = CreateDefaultSubobject<UWidgetComponent>("Character Locator Widget");
	PlayerLocatorWidget->SetupAttachment(RootComponent);
	PlayerLocatorWidget->SetWidgetSpace(EWidgetSpace::Screen);
	PlayerLocatorWidget->SetDrawSize(FVector2D(50, 75));
	PlayerLocatorWidget->SetRelativeLocation(FVector(0, 0, 200));
	
	CrashAttributes = CreateDefaultSubobject<UCrashAttributeSet>("Default Attributes");
	TimelineComponent = CreateDefaultSubobject<UTimelineComponent>("Timeline");

	static ConstructorHelpers::FObjectFinder<UGameplayEffect> AttributeSetup
		(TEXT("/Game/Blueprints/GAS/Effects/GE_DefaultAttributeEffect.GE_DefaultAttributeEffect"));
	
	DefaultAttributeEffect = AttributeSetup.Object->StaticClass();

	static ConstructorHelpers::FObjectFinder<USoundBase> DefaultDeathSound
		(TEXT("/Script/MetasoundEngine.MetaSoundSource'/Game/Blueprints/MetaSounds/CharacterDeath/MS_CharacterDeath.MS_CharacterDeath'"));

	CharacterAudioData.CharacterDeathEffect = DefaultDeathSound.Object;
	
	GetCapsuleComponent()->SetCapsuleRadius(50.f);
	
	AddDefaultAbilities();
	SetDefaultMesh();

	
}

void ACrashCharacter::SetDefaultMesh() const
{
	//Set the default mesh if not overriden in Blueprint
	if(GetMesh()->GetSkeletalMeshAsset() == nullptr)
	{
		static ConstructorHelpers::FObjectFinder<USkeletalMesh> SkeletalMesh //Gets the skeletal mesh from file path
			(TEXT("/Script/Engine.SkeletalMesh'/Game/Assets/PolygonFantasyRivals/Meshes/Characters/SK_Character_SpiritDemon.SK_Character_SpiritDemon'"));

		static ConstructorHelpers::FObjectFinder<UAnimBlueprint> AnimObject //Gets the Animation BP from file path
			(TEXT("/Script/Engine.AnimBlueprint'/Game/Blueprints/Characters/Animation/AnimBP_CrashCharacter.AnimBP_CrashCharacter'"));

		if(!SkeletalMesh.Succeeded() || !AnimObject.Succeeded()) 
		{
			UE_LOG(LogTemp, Error, TEXT("Default Crash Mesh and Animation Could not be found"));
			return;
		}

		//Set mesh and animation values and initialize default position and rotation of mesh 
		GetMesh()->SetSkeletalMeshAsset(SkeletalMesh.Object);
		GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -90.f));
		GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
		GetMesh()->SetAnimInstanceClass(AnimObject.Object->GeneratedClass);
	}
}

void ACrashCharacter::AddDefaultAbilities()
{
	DefaultAbilities.Add(UJumpAbility::StaticClass());
	DefaultAbilities.Add(UBlockAbility::StaticClass());
	DefaultAbilities.Add(USlideAbility::StaticClass());
	DefaultAbilities.Add(UAirAttackAbility::StaticClass());
}

void ACrashCharacter::CheckFallingDown()
{
	if(GetVelocity().Z < 0)
	{
		OnCharacterFallingDown.Broadcast();
		FallingDownTimerHandle.Invalidate();
		bIsFallingDown = true;
	}
}

// Called when the game starts or when spawned
void ACrashCharacter::BeginPlay()
{
	Super::BeginPlay();

	ApplyEffectToCrashCharacter(UGroundedEffect::StaticClass());
}

void ACrashCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if(TimelineComponent != nullptr)
	{
		TimelineComponent->TickComponent(DeltaSeconds, LEVELTICK_TimeOnly, nullptr);
	}
}

void ACrashCharacter::OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode)
{
	Super::OnMovementModeChanged(PrevMovementMode, PreviousCustomMode);

	switch (GetCharacterMovement()->MovementMode)
	{
	case MOVE_Falling:
		OnStartedFalling();
		break;

		default:
			return;
	}
}

void ACrashCharacter::OnStartedFalling()
{
	const UAirborneEffect* Effect = NewObject<UAirborneEffect>();
	const FGameplayEffectContextHandle ContextHandle = FGameplayEffectContextHandle();
	GetAbilitySystemComponent()->ApplyGameplayEffectToSelf(Effect, 0, ContextHandle);

	GetWorldTimerManager().SetTimer(FallingDownTimerHandle, this, &ACrashCharacter::CheckFallingDown, 0.1f, true);
}

void ACrashCharacter::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);

	ApplyEffectToCrashCharacter(UGroundedEffect::StaticClass());
	bIsKnockedBack = false;
	bIsFallingDown = false;

	const FGameplayTag LandedTag = FGameplayTag::RequestGameplayTag("Player.State.Grounded");
	FGameplayEventData Data;
	GetAbilitySystemComponent()->HandleGameplayEvent(LandedTag, &Data);
}

void ACrashCharacter::SetUpDefaultMovementValues()
{
	if(UCharacterMovementComponent* MovementComponent = GetCharacterMovement())
	{
		bUseControllerRotationYaw = false;
		JumpMaxCount = 10;

		MovementComponent->bOrientRotationToMovement = true;
		MovementComponent->bUseSeparateBrakingFriction = false;
		MovementComponent->MaxWalkSpeed = 900.f;
		MovementComponent->GravityScale = 2.5f;
		MovementComponent->MaxAcceleration = 5000.f;
		MovementComponent->JumpZVelocity = 1200.f;
		MovementComponent->BrakingFrictionFactor = 1.f;
		MovementComponent->BrakingDecelerationWalking = 500.f;
		MovementComponent->BrakingDecelerationFalling = 3.f;
		MovementComponent->AirControl = 0.2f;
		MovementComponent->AirControlBoostMultiplier = 0.5f;
		MovementComponent->AirControlBoostVelocityThreshold = 1.5f;
		MovementComponent->FallingLateralFriction = 1.0f;
		MovementComponent->bApplyGravityWhileJumping = false;
		MovementComponent->RotationRate = FRotator(0,1500.0f,0);

		MovementComponent->SetPlaneConstraintAxisSetting(EPlaneConstraintAxisSetting::Y);
		MovementComponent->SetPlaneConstraintNormal(FVector(0, 1, 0));
		MovementComponent->bConstrainToPlane = true;
		
	}
}

void ACrashCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	
	if(AbilityComponent)
		AbilityComponent->InitAbilityActorInfo(this, this);

	InitializeAttributes();
	GiveDefaultAbilities();
}

void ACrashCharacter::InitializeAttributes()
{
	if(AbilityComponent && DefaultAttributeEffect)
		ApplyEffectToCrashCharacter(DefaultAttributeEffect);
}

void ACrashCharacter::GiveDefaultAbilities()
{
	if(!AbilityComponent)
		return;
	
	for (auto StartAbility : DefaultAbilities)
	{
		AbilityComponent->GiveAbility(FGameplayAbilitySpec(StartAbility.GetDefaultObject(), 1, static_cast<int32>(StartAbility.GetDefaultObject()->AbilityInputID)));
	}
}


void ACrashCharacter::ApplyEffectToCrashCharacter(TSubclassOf<UGameplayEffect> Effect) const
{
	FGameplayEffectContextHandle EffectContext = AbilityComponent->MakeEffectContext();
	EffectContext.AddSourceObject(this);
	const FGameplayEffectSpecHandle SpecHandle = AbilityComponent->MakeOutgoingSpec(Effect, 1, EffectContext);
	
	if(SpecHandle.IsValid())
		FActiveGameplayEffectHandle GEHandle = AbilityComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
}

void ACrashCharacter::FaceActor(const AActor* TargetActor)
{
	const FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), TargetActor->GetActorLocation());
	SetActorRotation(FRotator(0, LookAtRotation.Yaw, 0));
}

void ACrashCharacter::KillCharacter()
{
	GetCharacterMovement()->SetMovementMode(MOVE_None);

	bIsStuned = false;
	bIsKnockedBack = false;
	bEndKnockback = true;
	bIsFallingDown = false;
	
	ApplyEffectToCrashCharacter(UDeathEffect::StaticClass());
	ApplyEffectToCrashCharacter(UDamageLivesInstant::StaticClass());

	UCameraSubsystem* CameraSubsystem = GetWorld()->GetSubsystem<UCameraSubsystem>();
	const float Trauma = 1;
	CameraSubsystem->ApplyCameraShake(Trauma, 2, 450, 350, 0.8, 1.5);

	UGameplayStatics::PlaySoundAtLocation(GetWorld(), CharacterAudioData.CharacterDeathEffect, GetActorLocation(), FRotator::ZeroRotator);
	
	if(GetCrashAttributeSet()->GetLives() <= 0)
	{
		OnCharacterKnockedOut.Broadcast();
		UE_LOG(LogTemp, Warning, TEXT("Character KO"));
		return;
	}

	FGameplayAbilitySpec RespawnAbility = FGameplayAbilitySpec(URespawnAbility::StaticClass());
	GetAbilitySystemComponent()->GiveAbilityAndActivateOnce(RespawnAbility);

	GetWorldTimerManager().SetTimer(RespawnTimerHandle, this, &ACrashCharacter::OnRespawnTimerFinished, RespawnDelay);
	
	OnCharacterDeath.Broadcast();
}

void ACrashCharacter::OnRespawnTimerFinished() //When respawn delay has finished the character will respawn
{
	FGameplayTagContainer DeathTagContainer;
	TArray<FString> DeathTags;
	DeathTags.Add("Player.State.Dead");
	
	UGameplayTagsManager::Get().RequestGameplayTagContainer(DeathTags, DeathTagContainer);
	AbilityComponent->RemoveActiveEffectsWithGrantedTags(DeathTagContainer);
}

