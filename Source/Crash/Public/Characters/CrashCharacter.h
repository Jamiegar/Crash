﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "GAS/CrashAbilitySystemComponent.h"
#include "GAS/CrashAttributeSet.h"
#include "Interfaces/CombatAbilities.h"
#include "Interfaces/CrashController.h"
#include "CrashCharacter.generated.h"

class UCharacterUIDataInfo;
class UWidgetComponent;
class UTimelineComponent;
class UKnockbackComponent;
class UCombatComponent;
class UCrashGameplayAbility;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCharacterKnockOut, ACrashCharacter*, KnockedOutCharacter);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnKillCharacter);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnFallingDown);

USTRUCT(Blueprintable)
struct FCharacterAudioData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Sound")
	USoundBase* CharacterDeathEffect;
};


USTRUCT(BlueprintType, Blueprintable)
struct FCharacterUIInfo
{
	GENERATED_BODY()
public:
	FCharacterUIInfo()
	{
		CharacterPortrait = nullptr;
		CharacterColor = FLinearColor::White;
		CharacterName = FText();
	}
	
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="UI Info")
	UTexture2D* CharacterPortrait = nullptr;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="UI Info")
	FLinearColor CharacterColor = FLinearColor::White;;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="UI Info")
	FText CharacterName = FText();
};

UCLASS(Abstract)
class CRASH_API ACrashCharacter : public ACharacter, public IAbilitySystemInterface, public ICombatAbilities, public ICrashController
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FOnKillCharacter OnCharacterDeath;

	UPROPERTY(BlueprintAssignable)
	FOnFallingDown OnCharacterFallingDown;

	UPROPERTY(BlueprintAssignable)
	FCharacterKnockOut OnCharacterKnockedOut;
		
	UFUNCTION(BlueprintCallable, Category="Kill")
	void KillCharacter();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Effects")
	FCharacterAudioData CharacterAudioData;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bCanMove = true;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsKnockedBack = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bIsShielded = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bIsStuned = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bEndKnockback;

	UPROPERTY(BlueprintReadOnly)
	bool bIsFallingDown = false;


protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Respawning")
	float RespawnDelay = 1.2;

	UPROPERTY()
	FTimerHandle RespawnTimerHandle;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Abilities")
	TSubclassOf<class UGameplayEffect> DefaultAttributeEffect;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Abilities")
	TArray<TSubclassOf<UCrashGameplayAbility>> DefaultAbilities;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category= "Abilities")
	UCrashAbilitySystemComponent* AbilityComponent;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Abilities")
	UCombatComponent* CombatComponent;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Knockback")
	UKnockbackComponent* KnockbackComponent;

	UPROPERTY(BlueprintReadOnly, Category="Timeline")
	UTimelineComponent* TimelineComponent;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="User Interface")
	UWidgetComponent* PlayerLocatorWidget;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	USceneComponent* MeshAttachment;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Crash User Interface")
	FCharacterUIInfo UIData = FCharacterUIInfo();
	
	UPROPERTY(BlueprintReadOnly, Category="Movement")
	FTimerHandle FallingDownTimerHandle;

private:
	UPROPERTY(EditDefaultsOnly)
	UCrashAttributeSet* CrashAttributes;
	
public:
	// Sets default values for this character's properties
	ACrashCharacter();
	void SetDefaultMesh() const;
	void AddDefaultAbilities();
	
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override { return AbilityComponent; }

	UFUNCTION(BlueprintCallable)
	UCrashAttributeSet* GetCrashAttributeSet() const {return CrashAttributes; };
	
	UKnockbackComponent* GetKnockbackComponent() const { return KnockbackComponent; }
	UTimelineComponent* GetTimelineComponent() const { return TimelineComponent; }
	USceneComponent* GetMeshAttachmentPoint() const {return  MeshAttachment; }

	UFUNCTION(BlueprintCallable)
	FCharacterUIInfo GetCharacterUIDataInfoUIData() { return UIData; }

	virtual void PossessedBy(AController* NewController) override;
	virtual void InitializeAttributes();
	virtual void GiveDefaultAbilities();
	
	void ApplyEffectToCrashCharacter(TSubclassOf<UGameplayEffect> Effect) const;

	UFUNCTION(BlueprintCallable)
	void FaceActor(const AActor* TargetActor);

	//////////////////////////////////////////////////////////////////////////
	// ICrashController
	//////////////////////////////////////////////////////////////////////////

	UFUNCTION(BlueprintCallable, Category="Controller")
	virtual APlayerController* GetCrashCharacterPlayerController() override;
	
	virtual void ApplyDynamicForceFeedback(float Intensity, float Duration, bool AffectsLeftLarge = true, bool AffectsLeftSmall = true, bool AffectsRightLarge= true, bool AffectsRightSmall = true) override;
	
	//////////////////////////////////////////////////////////////////////////
	// ICombatInterface
	//////////////////////////////////////////////////////////////////////////
	UFUNCTION(BlueprintCallable)
	virtual UCombatComponent* GetCombatComponent() override { return CombatComponent; }

protected:
	UFUNCTION(Category="Movement")
	void CheckFallingDown();
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetCharacterSpeed() { return GetVelocity().Length(); }
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode) override;
	virtual void OnStartedFalling();
	virtual void Landed(const FHitResult& Hit) override;
	virtual void SetUpDefaultMovementValues();

	UFUNCTION()
	virtual void OnRespawnTimerFinished();

};





