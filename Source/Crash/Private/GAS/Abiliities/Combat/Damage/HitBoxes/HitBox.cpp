// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Abiliities/Combat/Damage/HitBoxes/HitBox.h"
#include "Components/BoxComponent.h"


// Sets default values
AHitBox::AHitBox()
{
	PrimaryActorTick.bCanEverTick = true;

	HitBox = CreateDefaultSubobject<UBoxComponent>("HitBox");
	RootComponent = HitBox;
	HitBox->SetHiddenInGame(true);
	HitBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	HitBox->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
}

void AHitBox::ActivateHitBox()
{
	HitBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	bIsHitBoxActive = true;
}






