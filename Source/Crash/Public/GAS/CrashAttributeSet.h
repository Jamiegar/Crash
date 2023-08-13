// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "CrashAttributeSet.generated.h"

//Helper GAS Functions
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FUpdatePercent, float, Percent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FUpdateLives, float, Lives);


UCLASS()
class CRASH_API UCrashAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FUpdatePercent OnPercentUpdated;

	UPROPERTY(BlueprintAssignable)
	FUpdateLives OnLivesUpdated;

	
	ATTRIBUTE_ACCESSORS(UCrashAttributeSet, NumberOfJumps);
	ATTRIBUTE_ACCESSORS(UCrashAttributeSet, Percentage);
	ATTRIBUTE_ACCESSORS(UCrashAttributeSet, Lives);
	ATTRIBUTE_ACCESSORS(UCrashAttributeSet, Damage);
	ATTRIBUTE_ACCESSORS(UCrashAttributeSet, Weight);
	ATTRIBUTE_ACCESSORS(UCrashAttributeSet, Knockback);

	
protected:

	UCrashAttributeSet();
	
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	UPROPERTY(BlueprintReadOnly, Category="Atributes|Movement", meta=(AllowPrivateAccess = true))
	FGameplayAttributeData NumberOfJumps;
	
	UPROPERTY(BlueprintReadOnly, Category="Atributes|Heath / Damage", meta=(AllowPrivateAccess = true))
	FGameplayAttributeData Percentage;

	UPROPERTY(BlueprintReadOnly, Category="Atributes|Heath / Damage", meta=(AllowPrivateAccess = true))
	FGameplayAttributeData Lives;
	
	UPROPERTY(BlueprintReadOnly, Category="Atributes|Heath / Damage", meta=(AllowPrivateAccess = true))
	FGameplayAttributeData Damage;
	
	UPROPERTY(BlueprintReadOnly, Category="Atributes|Heath / Damage", meta=(AllowPrivateAccess = true))
	FGameplayAttributeData Weight;

	UPROPERTY(BlueprintReadOnly, Category="Atributes|Heath / Damage", meta=(AllowPrivateAccess = true))
	FGameplayAttributeData Knockback;
	
	

	
	
	


	
	
};
