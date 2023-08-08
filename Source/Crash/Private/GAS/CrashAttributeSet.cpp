// Fill out your copyright notice in the Description page of Project Settings.



#include "GAS/CrashAttributeSet.h"
#include "GameplayEffectExtension.h"

UCrashAttributeSet::UCrashAttributeSet()
{
}

void UCrashAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
	
	if(Data.EvaluatedData.Attribute == GetDamageAttribute()) //Check if the changed attribute is the damage attribute
	{
		SetPercentage(GetPercentage() + GetDamage()); //The new percentage is set by adding the damage to the percent
		UE_LOG(LogTemp, Warning, TEXT("Damage: %f"), GetDamage());
		UE_LOG(LogTemp, Warning, TEXT("New Percentage: %f"), GetPercentage());
		SetDamage(0.0f); //Damage attribute is set to 0 after it has been used 
	}
	else if(Data.EvaluatedData.Attribute == GetLivesAttribute())
	{
		SetPercentage(0.0f);
	}
}

