// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RespawnAnchor.generated.h"

class UNiagaraSystem;
class UBoxComponent;
class ACrashCharacter;
class UTimelineComponent;

UCLASS()
class CRASH_API ARespawnAnchor : public AActor
{
	GENERATED_BODY()

public:
	ARespawnAnchor();
	
	UFUNCTION(BlueprintCallable, Category="Spawning")
	void RespawnCharacter(ACrashCharacter* Character, FVector TargetPosition); 

	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category="Timeline")
	UTimelineComponent* GetTimelineComponent() const { return TimelineComponent; }


private:

	UPROPERTY(EditDefaultsOnly, Category="Mesh")
	UStaticMeshComponent* StaticMesh;

	UPROPERTY(EditDefaultsOnly, Category="Collision")
	UBoxComponent* BoxCollision;
	
	UPROPERTY(EditDefaultsOnly, Category="Timeline")
	UTimelineComponent* TimelineComponent;

	UPROPERTY(EditDefaultsOnly, Category="Destroy")
	UNiagaraSystem* DestroyedEffect;

	UPROPERTY(EditDefaultsOnly)
	FVector StartLocationOffset = FVector(0, 0, 2000);

	UPROPERTY(EditDefaultsOnly)
	float LifeTimeLength = 5.0f;
	
	UPROPERTY()
	FTimerHandle LifeTimeHandle;
	
	UPROPERTY()
	FVector StartLocation;

	UPROPERTY()
	FVector TargetLocation;
	
	UPROPERTY()
	UCurveFloat* FlyInCurve;

	UPROPERTY()
	ACrashCharacter* CrashCharacter;

	UFUNCTION()
	void TimelineUpdate(float InterpValue);

	UFUNCTION()
	void OnTimelineFinished();

	UFUNCTION()
	void OnLifeTimeTimerEnd();

	UFUNCTION()
	void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
};
