// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Abiliities/Combat/Damage/KnockBackAbility.h"

#include "GameplayTagsManager.h"
#include "Abilities/Tasks/AbilityTask_WaitDelay.h"
#include "Characters/CrashCharacter.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "AssetTypeActions/AssetDefinition_SoundBase.h"
#include "Characters/CombatComponents/KnockbackComponent.h"
#include "Components/AudioComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GAS/CrashAttributeSet.h"
#include "GAS/Abiliities/Combat/Damage/Data/KnockbackData.h"
#include "GAS/Effects/Respawn/InvincibilityEffect.h"
#include "Kismet/GameplayStatics.h"
#include "Subsystems/CameraSubsystem.h"

UKnockBackAbility::UKnockBackAbility()
{
	ActivationPolicy = ECrashActivationPolicy::OnSpawn;

	AbilityTags.AddTag(FGameplayTag::RequestGameplayTag("Player.Damaged.Knockback"));
	CancelAbilitiesWithTag.AddTag(FGameplayTag::RequestGameplayTag("Player.State.Stun"));

	static ConstructorHelpers::FObjectFinder<UAnimMontage> DefaultGetUp
		(TEXT("/Script/Engine.AnimMontage'/Game/Blueprints/Characters/Animation/Montages/GetUp/AnimMon_GetUp.AnimMon_GetUp'"));
	
	GetUpMontage = DefaultGetUp.Object;

	static ConstructorHelpers::FObjectFinder<USoundBase> DefaultKnockbackStartSound
		(TEXT("/Script/MetasoundEngine.MetaSoundSource'/Game/Blueprints/MetaSounds/CharacterAttacks/MS_KnockbackStartEffect.MS_KnockbackStartEffect'"));

	static ConstructorHelpers::FObjectFinder<USoundBase> DefaultKnockbackGroundSound
		(TEXT("/Script/MetasoundEngine.MetaSoundSource'/Game/Blueprints/MetaSounds/CharacterAttacks/MS_KnckbackEndEffect.MS_KnckbackEndEffect'"));

	KnockbackSoundData.KnockbackStartSoundEffect = DefaultKnockbackStartSound.Object;
	KnockbackSoundData.KnockbackEndSoundEffect = DefaultKnockbackGroundSound.Object;
}

void UKnockBackAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if(!CommitCrashAbility())
		CancelAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), false);
	

	FGameplayTag DataTag = FGameplayTag::RequestGameplayTag("Event.Data.Knockback");
	AsyncEventKnockbackData = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, DataTag);
	AsyncEventKnockbackData->EventReceived.AddUniqueDynamic(this, &UKnockBackAbility::OnReceivedKnockbackData);
	AsyncEventKnockbackData->Activate();
}

void UKnockBackAbility::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateCancelAbility)
{
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);
	
	TargetCharacter->bCanMove = true;
	TargetCharacter->GetKnockbackComponent()->StopFaceVelocity();
	
	FGameplayTagContainer KnockbackTagContainer;
	TArray<FString> Tags;
	Tags.Add("Player.Damaged.Knockback");
	UGameplayTagsManager::Get().RequestGameplayTagContainer(Tags, KnockbackTagContainer);
	
	BP_RemoveGameplayEffectFromOwnerWithGrantedTags(KnockbackTagContainer);
}

void UKnockBackAbility::OnReceivedKnockbackData(FGameplayEventData Payload)
{
	KnockbackData = Cast<UKnockbackData>(Payload.OptionalObject);

	if(!KnockbackData)
	{
		EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfoRef(), true, false);
		return;
	}
	
	FVector Direction = FVector(Payload.Target->GetActorLocation() - Payload.Instigator->GetActorLocation());
	Direction.Normalize();
	Direction += FVector(0, 0, 1);
	
	TargetCharacter = CastChecked<ACrashCharacter>(Payload.Target);
	
	if(const UCrashAttributeSet* Attribute = TargetCharacter->GetCrashAttributeSet())
	{
		KnockbackMag = Attribute->GetKnockback();

		if(KnockbackMag < KnockbackData->MinKnockbackMagnitude)
			KnockbackMag = KnockbackData->MinKnockbackMagnitude;

		FVector LaunchVelocity = Direction * KnockbackMag;
		LaunchVelocity.X *= KnockbackData->HorizontalForce;
		LaunchVelocity.Z *= KnockbackData->UpwardForce;
		
		TargetCharacter->LaunchCharacter(LaunchVelocity, true, true);
		TargetCharacter->bIsKnockedBack = true;
		TargetCharacter->GetKnockbackComponent()->StartFaceVelocityDirection();

		KnockbackAudio = UGameplayStatics::SpawnSoundAttached(KnockbackSoundData.KnockbackStartSoundEffect, GetActorInfo().OwnerActor.Get()->GetRootComponent());

		UCameraSubsystem* CameraSubsystem = GetWorld()->GetSubsystem<UCameraSubsystem>();
		const float Trauma = KnockbackMag / 2500;
		CameraSubsystem->ApplyCameraShake(Trauma, 1.5, 400, 75, 0.5, 1.5);

		const FGameplayTag GroundedTag = FGameplayTag::RequestGameplayTag("Player.State.Grounded");
		AsyncEventWaitUntilGrounded = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, GroundedTag, nullptr, true, true);
		AsyncEventWaitUntilGrounded->EventReceived.AddUniqueDynamic(this, &UKnockBackAbility::OnCharacterGrounded);
		AsyncEventWaitUntilGrounded->Activate();

		
	}
	
}

void UKnockBackAbility::OnCharacterGrounded(FGameplayEventData Payload)
{
	WaitAsyncKncokbackEnd = UAbilityTask_WaitDelay::WaitDelay(this, KnockbackData->KnockbackGroundedDuration);
	WaitAsyncKncokbackEnd->OnFinish.AddUniqueDynamic(this, &UKnockBackAbility::OnKnockbackGroundedFinished);
	WaitAsyncKncokbackEnd->Activate();

	TargetCharacter->bCanMove = false;

	KnockbackAudio->FadeOut(0.5, 0);
	UGameplayStatics::SpawnSoundAtLocation(GetWorld(), KnockbackSoundData.KnockbackEndSoundEffect, GetActorInfo().OwnerActor.Get()->GetActorLocation());

	UCameraSubsystem* CameraSubsystem = GetWorld()->GetSubsystem<UCameraSubsystem>();
	const float Trauma = KnockbackMag / 1000; 
	CameraSubsystem->ApplyCameraShake(Trauma, 2.5f, 300, 45, 0.4, 0.5);
}

void UKnockBackAbility::OnKnockbackGroundedFinished()
{
	FOnMontageEnded Delegate = FOnMontageEnded::CreateUObject(this, &UKnockBackAbility::GetupMontageFinished);
	PlayAnimationMontageToOwningActor(GetUpMontage, Delegate);
	
	TargetCharacter->GetKnockbackComponent()->StopFaceVelocity();
}

void UKnockBackAbility::GetupMontageFinished(UAnimMontage* Montage, bool bInterrupted)
{
	TargetCharacter->bCanMove = true;
	UE_LOG(LogTemp, Warning, TEXT("Can Move"));
	
	FGameplayTagContainer KnockbackTagContainer;
	TArray<FString> Tags;
	Tags.Add("Player.Damaged.Knockback");
	UGameplayTagsManager::Get().RequestGameplayTagContainer(Tags, KnockbackTagContainer);

	BP_RemoveGameplayEffectFromOwnerWithGrantedTags(KnockbackTagContainer);
	
	EndCrashAbility();
}


