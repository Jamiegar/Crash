// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Abiliities/Combat/Basic/BlockAbility.h"

#include "NiagaraFunctionLibrary.h"
#include "Abilities/Tasks/AbilityTask_WaitInputRelease.h"
#include "Characters/CrashCharacter.h"
#include "NiagaraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

UBlockAbility::UBlockAbility()
{
	ActivationPolicy = ECrashActivationPolicy::OnInputTriggered;
	AbilityInputID = EAbilityInputID::Block;

	ActivationOwnedTags.AddTag(FGameplayTag::RequestGameplayTag("Player.State.Blocking"));
	ActivationBlockedTags.AddTag(FGameplayTag::RequestGameplayTag("Player.State.Airborne"));
	ActivationBlockedTags.AddTag(FGameplayTag::RequestGameplayTag("Player.Attack"));
	

	static ConstructorHelpers::FObjectFinder<UStaticMesh> ShieldDefaultMesh
		(TEXT("/Script/Engine.StaticMesh'/Game/Assets/CustomMeshes/SheildSphere.SheildSphere'"));
	ShieldMesh = ShieldDefaultMesh.Object;

	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> NiagaraDefaultShieldVFX
		(TEXT("/Script/Niagara.NiagaraSystem'/Game/Assets/Vefects/Flipbook_VFX/Shared/Particles/Charge/NS_Charge_01.NS_Charge_01'"));
	NiagaraShieldVFX = NiagaraDefaultShieldVFX.Object;
	
}

void UBlockAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	AsyncWaitInputRelease = UAbilityTask_WaitInputRelease::WaitInputRelease(this);
	AsyncWaitInputRelease->OnRelease.AddUniqueDynamic(this, &UBlockAbility::OnInputRelease);
	AsyncWaitInputRelease->Activate();

	ACrashCharacter* Character = CastChecked<ACrashCharacter>(GetActorInfo().AvatarActor);
	
	/*ShieldMeshComponent = NewObject<UStaticMeshComponent>(Character, UStaticMeshComponent::StaticClass(), TEXT("Shield Mesh"));
	ShieldMeshComponent->RegisterComponent();
	ShieldMeshComponent->AttachToComponent(Character->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform);
	ShieldMeshComponent->AddRelativeLocation(FVector(0, 0, 50));
	ShieldMeshComponent->CreationMethod = EComponentCreationMethod::Instance;

	if(ShieldMesh)
		ShieldMeshComponent->SetStaticMesh(ShieldMesh);*/
	
	Character->bIsShielded = true;
	ActiveShieldVFX = UNiagaraFunctionLibrary::SpawnSystemAttached(NiagaraShieldVFX, Character->GetMesh(), "spine_01",
		FVector(), FRotator(), EAttachLocation::Type::KeepRelativeOffset, true);

	Character->GetCharacterMovement()->SetMovementMode(MOVE_None);
}

void UBlockAbility::OnInputRelease(float TimeHeld)
{
	//ShieldMeshComponent->DestroyComponent();
	ACrashCharacter* Character = CastChecked<ACrashCharacter>(GetActorInfo().AvatarActor);
	Character->bIsShielded = false;
	ActiveShieldVFX->DestroyComponent();

	Character->GetCharacterMovement()->SetMovementMode(MOVE_Walking);
	
	EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfoRef(), true, false);
}
