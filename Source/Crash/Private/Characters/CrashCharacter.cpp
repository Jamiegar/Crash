// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/CrashCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GAS/CrashAttributeSet.h"
#include "GAS/Abiliities/Movement/JumpAbility.h"
#include "GAS/Effects/AirborneEffect.h"
#include "GAS/Effects/GroundedEffect.h"


// Sets default values
ACrashCharacter::ACrashCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationYaw = false;

	AbilityComponent = CreateDefaultSubobject<UAbilitySystemComponent>("Ability Component");
	Attributes = CreateDefaultSubobject<UCrashAttributeSet>("Attributes");

	DefaultAbilities.Add(UJumpAbility::StaticClass());

	JumpMaxCount = 2;
}

UAbilitySystemComponent* ACrashCharacter::GetAbilitySystemComponent() const
{
	return AbilityComponent;
}

// Called when the game starts or when spawned
void ACrashCharacter::BeginPlay()
{
	Super::BeginPlay();
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


// Called every frame
void ACrashCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
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
	{
		ApplyEffectToCrashCharacter(DefaultAttributeEffect);
	}
}

void ACrashCharacter::GiveDefaultAbilities()
{
	if(!AbilityComponent)
		return;
	
	for (auto StartAbility : DefaultAbilities)
	{
		AbilityComponent->GiveAbility(FGameplayAbilitySpec(StartAbility.GetDefaultObject(), 1, StartAbility.GetDefaultObject()->AbilityInputID));
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

