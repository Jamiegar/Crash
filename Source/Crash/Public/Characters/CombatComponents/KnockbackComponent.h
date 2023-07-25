// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "KnockbackComponent.generated.h"


class ACrashCharacter;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CRASH_API UKnockbackComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UKnockbackComponent();

	UFUNCTION(BlueprintCallable, Category="Knockback")
	void ApplyKnockbackToCharacter(const FVector LaunchDirection, const float Magnitude);

	UFUNCTION(BlueprintCallable, Category="Knockback")
	void StartScrew(UCurveFloat* ScrewCurve);

	UFUNCTION(BlueprintCallable, Category="Knockback")
	void StopScrew();

	UFUNCTION()
	void ScrewTimelineUpdate(float InterpValue);

	UFUNCTION()
	void StartFaceVelocityDirection();

	UFUNCTION()
	void StopFaceVelocity();

private:
	FTimerHandle FaceVectorDirectionTimerHandle;

	UFUNCTION()
	void FaceVelocityDirection() const;
	
	virtual void BeginPlay() override;
	
	ACrashCharacter* OwnerCharacter;
	
};
