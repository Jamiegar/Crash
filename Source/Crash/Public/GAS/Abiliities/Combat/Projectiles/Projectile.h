// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

class UCapsuleComponent;
class UNiagaraComponent;
class UProjectileMovementComponent;

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

	UPROPERTY()
	FGameplayEffectSpecHandle ProjectileHitEffectHandle; 
	
	
public:
	// Sets default values for this actor's properties
	AProjectile();

	UFUNCTION(BlueprintCallable, Category="Damaging")
	void SetDamagingEffectHandle(const FGameplayEffectSpecHandle DamagingSpecHandle) { ProjectileHitEffectHandle = DamagingSpecHandle; }
	

protected:
	void SetDefaultProjectileMovementValues();

	UFUNCTION()
	void OnCollisionComponentOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	virtual FGameplayEffectSpecHandle CreateDefaultEffect(UAbilitySystemComponent* AbilityComponent);
	
};
