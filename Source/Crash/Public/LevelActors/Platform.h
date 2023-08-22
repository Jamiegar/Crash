// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Platform.generated.h"

class UBoxComponent;

UCLASS()
class CRASH_API APlatform : public AActor
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Mesh")
	UStaticMeshComponent* PlatformMesh;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Collision")
	UBoxComponent* OverlappingCollision;
	
	APlatform();
	virtual void BeginPlay() override;


private:
	UFUNCTION()
	void OnOverlapWithPlatform(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION()
	void OnEndOverlapWithPlatform(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
