// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/CombatComponents/KnockbackComponent.h"
#include "Characters/CrashCharacter.h"


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







