// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Abiliities/Movement/PhasePlatform.h"

#include "Abilities/Tasks/AbilityTask_WaitInputPress.h"
#include "Abilities/Tasks/AbilityTask_WaitInputRelease.h"
#include "Characters/CrashPlayerCharacter.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "LevelActors/Platform.h"


UPhasePlatform::UPhasePlatform()
{
	AbilityInputID = EAbilityInputID::DownMovement;
}

void UPhasePlatform::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if(!CommitCrashAbility())
		return;

	AsyncWaitInputReleased = UAbilityTask_WaitInputRelease::WaitInputRelease(this, true);
	AsyncWaitInputReleased->OnRelease.AddUniqueDynamic(this, &UPhasePlatform::OnAbilityInputReleased);
	AsyncWaitInputReleased->Activate();

	ACrashPlayerCharacter* Owner = CastChecked<ACrashPlayerCharacter>(GetActorInfo().OwnerActor.Get());
	Owner->LaunchCharacter(FVector::DownVector * DownForce, false, true);
	GetWorld()->GetTimerManager().SetTimer(PlatformDetectionTimerHandle, this, &UPhasePlatform::DetectPlatform, 0.1f, true);
}

void UPhasePlatform::DetectPlatform()
{
	ACrashPlayerCharacter* Owner = CastChecked<ACrashPlayerCharacter>(GetActorInfo().OwnerActor.Get());

	
	FHitResult Hit;
	FVector Start = Owner->GetActorLocation();
	FVector End = Start + (FVector::DownVector * DownTraceDistance);
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(Owner);
	
	bool ValidHit = GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_WorldStatic, QueryParams);
	
	if(!ValidHit)
		return;

	if(Hit.HitObjectHandle.DoesRepresentClass(APlatform::StaticClass()))
	{
		Owner->GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Overlap);
	}
	
}


void UPhasePlatform::OnAbilityInputReleased(float DownDuration)
{
	GetWorld()->GetTimerManager().ClearTimer(PlatformDetectionTimerHandle);
	EndCrashAbility();
}
