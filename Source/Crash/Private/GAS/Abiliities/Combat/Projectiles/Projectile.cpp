// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Abiliities/Combat/Projectiles/Projectile.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Characters/Interfaces/CombatAbilities.h"
#include "Components/AudioComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GAS/CrashGameplayTags.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"


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

void AProjectile::BeginPlay()
{
	Super::BeginPlay();

	LoopingProjectileAudioComponent = UGameplayStatics::SpawnSoundAttached(ProjectileSoundData.ProjectileFlyingLoop, RootComponent);
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
	
	const IAbilitySystemInterface* AbilitySystemInterface = Cast<IAbilitySystemInterface>(GetInstigator());
	const ICombatAbilities* HitCharacter = Cast<ICombatAbilities>(OtherActor);
	
	if(OtherActor != GetInstigator() && AbilitySystemInterface && HitCharacter)
	{
		UAbilitySystemComponent* ASC = AbilitySystemInterface->GetAbilitySystemComponent();

		FGameplayAbilityTargetData_ActorArray* NewData = new FGameplayAbilityTargetData_ActorArray();
		NewData->TargetActorArray.Add(OtherActor);
		
		const FGameplayAbilityTargetDataHandle Handle(NewData);
		
		FGameplayEventData* Data = new FGameplayEventData;
		Data->Instigator = GetInstigator();
		Data->Target = OtherActor;
		Data->TargetData = Handle;

		const FCrashGameplayTags& GameTags = FCrashGameplayTags::Get();
		ASC->HandleGameplayEvent(GameTags.PlayerDamaged, Data);

		if(LoopingProjectileAudioComponent)
			LoopingProjectileAudioComponent->EndPlay(EEndPlayReason::Destroyed);
		
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), ProjectileSoundData.ProjectileHit, GetActorLocation(), FRotator::ZeroRotator);
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), OnDestroyedVFX, GetActorLocation(), UKismetMathLibrary::MakeRotFromX(GetActorForwardVector() * -1));

		Destroy();
	}
	
}






