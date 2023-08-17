// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

class UNiagaraSystem;
class UCapsuleComponent;
class UNiagaraComponent;
class UProjectileMovementComponent;

USTRUCT(Blueprintable)
struct FProjectileSoundData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Sound")
	USoundBase* ProjectileFlyingLoop;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Sound")
	USoundBase* ProjectileHit;
};


UCLASS(Blueprintable)
class CRASH_API AProjectile : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Projectile")
	UCapsuleComponent* CollisionComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Projectile")
	UStaticMeshComponent* ProjectileMesh;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Projectile")
	UNiagaraComponent* NiagaraComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Projectile")
	UProjectileMovementComponent* ProjectileComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Effects")
	UNiagaraSystem* OnDestroyedVFX;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Effects")
	FProjectileSoundData ProjectileSoundData;

	
	
public:
	// Sets default values for this actor's properties
	AProjectile();

	virtual void BeginPlay() override;


protected:
	void SetDefaultProjectileMovementValues();

	UFUNCTION()
	void OnCollisionComponentOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY()
	UAudioComponent* LoopingProjectileAudioComponent;
};
