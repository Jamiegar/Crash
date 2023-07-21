﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/CombatComponents/KnockbackComponent.h"
#include "Characters/CrashCharacter.h"
#include "Components/TimelineComponent.h"
#include "Kismet/KismetMathLibrary.h"


UKnockbackComponent::UKnockbackComponent()
{

}

void UKnockbackComponent::ApplyKnockbackToCharacter(const FVector LaunchDirection, const float Magnitude)
{
	if(ACrashCharacter* Character = Cast<ACrashCharacter>(GetOwner()))
	{
		Character->LaunchCharacter(LaunchDirection * Magnitude, true, true);
	}
}

void UKnockbackComponent::StartScrew(UCurveFloat* ScrewCurve)
{
	FOnTimelineFloat TimelineFloat;
	UTimelineComponent* Timeline = OwnerCharacter->GetTimelineComponent();
	Timeline->SetLooping(true);
	TimelineFloat.BindUFunction(this, "ScrewTimelineUpdate");
	Timeline->AddInterpFloat(ScrewCurve, TimelineFloat);
	Timeline->PlayFromStart();
	
}

void UKnockbackComponent::StopScrew()
{
	UTimelineComponent* Timeline = OwnerCharacter->GetTimelineComponent();
	Timeline->Stop();

	OwnerCharacter->GetMeshAttachmentPoint()->SetRelativeRotation(FRotator(0,0,0));
}

void UKnockbackComponent::ScrewTimelineUpdate(float InterpValue)
{
	const FRotator DeltaRotator = FRotator(0.5, 0, 0.0);
	const FRotator Current = OwnerCharacter->GetMeshAttachmentPoint()->GetRelativeRotation();
	const FRotator Rotation = UKismetMathLibrary::ComposeRotators(Current, DeltaRotator);

	const FRotator NewRotation = FMath::RInterpConstantTo(Current, Rotation, GetWorld()->GetDeltaSeconds(), 3.5);
	
	OwnerCharacter->GetMeshAttachmentPoint()->SetRelativeRotation(NewRotation);
}

void UKnockbackComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = CastChecked<ACrashCharacter>(GetOwner());
}








