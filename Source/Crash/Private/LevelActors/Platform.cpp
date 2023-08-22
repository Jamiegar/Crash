// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelActors/Platform.h"

#include "Characters/CrashCharacter.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"


// Sets default values
APlatform::APlatform()
{
	PrimaryActorTick.bCanEverTick = true;

	PlatformMesh = CreateDefaultSubobject<UStaticMeshComponent>("Platform Mesh");
	PlatformMesh->SetupAttachment(RootComponent);
	PlatformMesh->SetCollisionObjectType(ECC_WorldStatic);
	
	OverlappingCollision = CreateDefaultSubobject<UBoxComponent>("Overlapping Collision");
	OverlappingCollision->SetupAttachment(PlatformMesh);
	OverlappingCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	OverlappingCollision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	
}

void APlatform::BeginPlay()
{
	Super::BeginPlay();

	OverlappingCollision->OnComponentBeginOverlap.AddUniqueDynamic(this, &APlatform::OnOverlapWithPlatform);
	OverlappingCollision->OnComponentEndOverlap.AddUniqueDynamic(this, &APlatform::OnEndOverlapWithPlatform);
}

void APlatform::OnOverlapWithPlatform(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	const ACrashCharacter* Character = Cast<ACrashCharacter>(OtherActor);
	if(!Character)
		return;
	
	Character->GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Overlap);
}

void APlatform::OnEndOverlapWithPlatform(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	const ACrashCharacter* Character = Cast<ACrashCharacter>(OtherActor);
	if(!Character)
		return;

	Character->GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
}



