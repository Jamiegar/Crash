// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/CrashCharacter.h"
#include "GAS/CrashAttributeSet.h"


// Sets default values
ACrashCharacter::ACrashCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AbilityComponent = CreateDefaultSubobject<UAbilitySystemComponent>("Ability Component");
	Attributes = CreateDefaultSubobject<UCrashAttributeSet>("Attributes");
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
		FGameplayEffectContextHandle EffectContext = AbilityComponent->MakeEffectContext();
		EffectContext.AddSourceObject(this);
		const FGameplayEffectSpecHandle SpecHandle = AbilityComponent->MakeOutgoingSpec(DefaultAttributeEffect, 1, EffectContext);

		if(SpecHandle.IsValid())
			FActiveGameplayEffectHandle GEHandle = AbilityComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
	}
}

void ACrashCharacter::GiveDefaultAbilities()
{
	if(!AbilityComponent)
		return;
	
	for (auto StartAbility : DefaultAbilities)
	{
		AbilityComponent->GiveAbility(FGameplayAbilitySpec(StartAbility.GetDefaultObject(), 1, 0));
	}
}

