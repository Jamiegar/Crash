// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Abiliities/Combat/Projectiles/Projectile.h"

#include "NiagaraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"


// Sets default values
AProjectile::AProjectile()
{
	PrimaryActorTick.bCanEverTick = true;

	CollisionComponent = CreateDefaultSubobject<UCapsuleComponent>("Capsule Collision Component");
	
	RootComponent = CollisionComponent;

	NiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>("Niagara Component");
	NiagaraComponent->SetupAttachment(RootComponent);
	
	ProjectileComponent = CreateDefaultSubobject<UProjectileMovementComponent>("Projectile Component");

	
}

void AProjectile::SetDefaultProjectileMovementValues()
{
	ProjectileComponent->InitialSpeed = 2000;
	ProjectileComponent->MaxSpeed = 2000;
	ProjectileComponent->ProjectileGravityScale = 0;
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

