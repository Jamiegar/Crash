// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

class UNiagaraComponent;
class UCapsuleComponent;
class UProjectileMovementComponent;

UCLASS(Blueprintable)
class CRASH_API AProjectile : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AProjectile();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Projectile")
	UCapsuleComponent* CollisionComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Projectile")
	UNiagaraComponent* NiagaraComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Projectile")
	UProjectileMovementComponent* ProjectileComponent;
	
	void SetDefaultProjectileMovementValues();
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
