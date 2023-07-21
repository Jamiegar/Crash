// Fill out your copyright notice in the Description page of Project Settings.


#include "KillZone/KillZone.h"

#include "Characters/CrashCharacter.h"
#include "Components/BoxComponent.h"

AKillZone::AKillZone()
{
	KillZoneBounds = CreateDefaultSubobject<UBoxComponent>("Kill Zone Box");
	KillZoneBounds->SetLineThickness(5);
	KillZoneBounds->ShapeColor = FColor::Red;
}

void AKillZone::NotifyActorEndOverlap(AActor* OtherActor)
{
	Super::NotifyActorEndOverlap(OtherActor);

	if(ACrashCharacter* Character = Cast<ACrashCharacter>(OtherActor))
		Character->KillCharacter();
	
}
