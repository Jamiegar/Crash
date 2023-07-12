// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/CrashCharacter.h"
#include "Characters/CombatComponents/BasicCombatComponent.h"
#include "Characters/CombatComponents/KnockbackComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GAS/Abiliities/Combat/Basic/BlockAbility.h"
#include "GAS/Abiliities/Combat/Basic/SlideAbility.h"
#include "GAS/Abiliities/Movement/JumpAbility.h"
#include "GAS/Effects/AirborneEffect.h"
#include "GAS/Effects/GroundedEffect.h"


// Sets default values
ACrashCharacter::ACrashCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	ACrashCharacter::SetUpDefaultMovementValues();
	
	AbilityComponent = CreateDefaultSubobject<UCrashAbilitySystemComponent>("Ability Component");
	BasicCombatComponent = CreateDefaultSubobject<UBasicCombatComponent>("Basic Combat Component");
	KnockbackComponent = CreateDefaultSubobject<UKnockbackComponent>("Knockback Component");

	CrashAttributes = CreateDefaultSubobject<UCrashAttributeSet>("Default Attributes");
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
	
	if(BasicCombatComponent)
	{
		for (auto BasicCombat : BasicCombatComponent->BasicCombatAbilities)
			DefaultAbilities.Add(BasicCombat);
	}
}

UCrashAttributeSet* ACrashCharacter::GetCrashAttributeSet() const
{
	return CrashAttributes;
}

// Called when the game starts or when spawned
void ACrashCharacter::BeginPlay()
{
	Super::BeginPlay();

	ApplyEffectToCrashCharacter(UGroundedEffect::StaticClass());
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
}

void ACrashCharacter::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);

	ApplyEffectToCrashCharacter(UGroundedEffect::StaticClass());
	
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


