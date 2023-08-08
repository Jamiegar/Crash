// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Abiliities/Combat/Projectiles/Projectile.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "NiagaraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GAS/Effects/DamageBasicInstant.h"


// Sets default values
AProjectile::AProjectile()
{
	PrimaryActorTick.bCanEverTick = true;

	CollisionComponent = CreateDefaultSubobject<UCapsuleComponent>("Collision Component");
	CollisionComponent->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
	CollisionComponent->OnComponentBeginOverlap.AddUniqueDynamic(this, &AProjectile::OnCollisionComponentOverlap);
	SetRootComponent(CollisionComponent);
	
	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>("Projectile Mesh");
	ProjectileMesh->SetupAttachment(RootComponent);
	ProjectileMesh->SetRelativeRotation(FRotator(0, -90, 0));
	ProjectileMesh->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);

	NiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>("Niagara Component");
	NiagaraComponent->SetupAttachment(RootComponent);
	
	ProjectileComponent = CreateDefaultSubobject<UProjectileMovementComponent>("Projectile Component");

	SetDefaultProjectileMovementValues();
	AActor::SetLifeSpan(5.0f);
}

void AProjectile::SetDefaultProjectileMovementValues()
{
	ProjectileComponent->InitialSpeed = 2000;
	ProjectileComponent->MaxSpeed = 2000;
	ProjectileComponent->ProjectileGravityScale = 0;
}

void AProjectile::OnCollisionComponentOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(!OtherActor || GetInstigator() == nullptr)
		return;
	
	const IAbilitySystemInterface* AbilitySystemInterface = Cast<IAbilitySystemInterface>(OtherActor);

	if(OtherActor != GetInstigator() && AbilitySystemInterface)
	{
		UAbilitySystemComponent* ASC = AbilitySystemInterface->GetAbilitySystemComponent();

		if(!ProjectileHitEffectHandle.IsValid())
			CreateDefaultEffect(ASC);
			
		ASC->ApplyGameplayEffectSpecToSelf(*ProjectileHitEffectHandle.Data.Get());
		Destroy();
	}
	
}

FGameplayEffectSpecHandle AProjectile::CreateDefaultEffect(UAbilitySystemComponent* AbilityComponent)
{
	const FGameplayEffectContextHandle EffectContext = AbilityComponent->MakeEffectContext();
	ProjectileHitEffectHandle = AbilityComponent->MakeOutgoingSpec(UDamageBasicInstant::StaticClass(), 0, EffectContext);

	return ProjectileHitEffectHandle;
}




