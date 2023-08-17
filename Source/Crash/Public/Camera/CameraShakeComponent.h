// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CameraShakeComponent.generated.h"


class UCameraComponent;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CRASH_API UCameraShakeComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	// Sets default values for this component's properties
	UCameraShakeComponent();
	
	UFUNCTION(BlueprintCallable, Category="Shake")
	void SetShakeActive(bool NewActive);
	
	UFUNCTION(BlueprintCallable, Category="Shake")
	void SetTrauma(float NewTrauma) { Trauma = FMath::Clamp(NewTrauma, 0, 1); }

	UFUNCTION(BlueprintCallable, Category="Shake")
	void AddTrauma (float AdditionalTrauma) { Trauma = FMath::Clamp(Trauma + AdditionalTrauma, 0, 1); }

	UFUNCTION(BlueprintCallable, Category="Shake")
	void SetTraumaMultiplier(float Multiplier) { TraumaMultiplier = Multiplier; }

	UFUNCTION(BlueprintCallable, Category="Shake")
	void SetTraumaMagnitude(float Magnitude) { TraumaMagnitude = Magnitude; }

	UFUNCTION(BlueprintCallable, Category="Shake")
	void SetTraumaRotationalMagnitude(float RotationalMag) { TraumaRotationalMagnitude = RotationalMag; }

	UFUNCTION(BlueprintCallable, Category="Shake")
	void SetTraumaDepthMagnitude(float DepthMag) { TraumaDepthMagnitude = DepthMag; }

	UFUNCTION(BlueprintCallable, Category="Shake")
	void SetDecay(float NewDecay) { Decay = NewDecay; }
	
	UFUNCTION(BlueprintCallable, Category="Shake")
	float GetTrauma() const { return Trauma; }
	

	
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Setter("SetShakeActive"), Category="Shake")
	bool IsShakeActive = false;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Shake", meta=(ClampMin = 0, ClampMax = 1))
	float Trauma = 0;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Shake")
	float TraumaMultiplier = 5;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Shake")
	float TraumaMagnitude = 10.0f;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Shake")
	float TraumaRotationalMagnitude = 0.7f;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Shake")
	float TraumaDepthMagnitude = 0.0f;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Shake")
	float Decay = 1.3f;

	UPROPERTY()
	float TimeCounter = 0;

private:
	UPROPERTY()
	UCameraComponent* CameraComponent;

	float GetPerlinNoiseValue(const float Seed) const;
	FVector GetNoiseVector();
	FRotator GetNoiseRotator();

	
};
