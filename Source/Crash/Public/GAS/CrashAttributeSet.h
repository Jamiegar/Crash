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
	

UCLASS()
class CRASH_API UCrashAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UCrashAttributeSet();
	
	UPROPERTY(BlueprintReadOnly, Category="Character Atributes")
	FGameplayAttributeData NumberOfJumps;
	ATTRIBUTE_ACCESSORS(UCrashAttributeSet, NumberOfJumps);

	UPROPERTY(BlueprintReadOnly, Category="Character Atributes")
	FGameplayAttributeData Percentage;
	ATTRIBUTE_ACCESSORS(UCrashAttributeSet, Percentage);

	UPROPERTY(BlueprintReadOnly, Category="Character Atributes")
    FGameplayAttributeData Lives;
    ATTRIBUTE_ACCESSORS(UCrashAttributeSet, Lives);


	
	
};
