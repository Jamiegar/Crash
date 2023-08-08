// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CombatAbilities.generated.h"

class UCombatComponent;
// This class does not need to be modified.
UINTERFACE(MinimalAPI, meta = (CannotImplementInterfaceInBlueprint))
class UCombatAbilities : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class CRASH_API ICombatAbilities
{
	GENERATED_BODY()

public:

	UFUNCTION(Category="Combat Component")
	virtual UCombatComponent* GetCombatComponent() = 0;

	UFUNCTION(Category="Projectie")
	virtual FTransform GetProjectileSpawnTransform() { return ProjectileSpawnTransform; };

	UFUNCTION(Category="Projectile")
	virtual void SetProjectileSpawnTransform(FTransform Transform) { ProjectileSpawnTransform = Transform; }

protected:
	FTransform ProjectileSpawnTransform;
	
};
