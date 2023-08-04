// Fill out your copyright notice in the Description page of Project Settings.


#include "Menus/AbilityMapping/AbilityActionMap.h"
#include "SlateOptMacros.h"
#include "Menus/AbilityMapping/InputAbilityMappingWidget.h"
#include "InputAction.h"
#include "Characters/CrashPlayerCharacter.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION


void SAbilityActionMap::Construct(const FArguments& InArgs)
{
	OnInputSelectedAbility = InArgs._OnInputSelectedAbility;
	SetInputActions(InArgs._AbilityType);
	GetImagePaths();


	
	ChildSlot
	[
		SNew(SVerticalBox)
		+SVerticalBox::Slot()
		.AutoHeight().VAlign(VAlign_Center) .HAlign(HAlign_Center)
		[
			CreateInputAbilityMappingWidget(IA_UP, InputImage_Up)
		]
		+SVerticalBox::Slot()
		.AutoHeight().VAlign(VAlign_Center) .HAlign(HAlign_Center)
		[
			SNew(SHorizontalBox)
			+SHorizontalBox::Slot()
			.AutoWidth() .VAlign(VAlign_Center) .HAlign(HAlign_Center)
			[
				CreateInputAbilityMappingWidget(IA_Left, InputImage_Left)
			]
			+SHorizontalBox::Slot()
			.AutoWidth() .VAlign(VAlign_Center) .HAlign(HAlign_Center)
			[
				CreateInputAbilityMappingWidget(IA_Neutral, InputImage_Neutral)
			]
			+SHorizontalBox::Slot()
			.AutoWidth() .VAlign(VAlign_Center) .HAlign(HAlign_Center)
			[
				CreateInputAbilityMappingWidget(IA_Right, InputImage_Right)
			]
		]
		+SVerticalBox::Slot()
		.AutoHeight().VAlign(VAlign_Center) .HAlign(HAlign_Center)
		[
			CreateInputAbilityMappingWidget(IA_Down, InputImage_Down)
		]
	];

	
	
}

void SAbilityActionMap::UpdateActionMap(ACrashPlayerCharacter* SelectedCharacter)
{
	for (auto AbilityAction : InputAbilityMappingWidgets)
	{
		AbilityAction.Get()->UpdateCharacterAbilityMap(SelectedCharacter->GetInputAbilityMap());
	}
}

void SAbilityActionMap::GetImagePaths()
{
	const FString Content = FPaths::ProjectContentDir() / TEXT("CharacterTool") / TEXT("Resources");

	InputImage_Down = *FString(Content + TEXT("/InputIcons/LS_Down.png"));
	InputImage_Left = *FString(Content + TEXT("/InputIcons/LS_Left.png"));
	InputImage_Right = *FString(Content + TEXT("/InputIcons/LS_Right.png"));
	InputImage_Up = *FString(Content + TEXT("/InputIcons/LS_Up.png"));
	InputImage_Neutral = *FString(Content + TEXT("/InputIcons/LS_Neutral.png"));
	
}

void SAbilityActionMap::SetInputActions(EActionAbilityType Type)
{
	switch(Type)
	{
		case EActionAbilityType::Basic:
			IA_Left = LoadInputAction("/Game/Blueprints/Characters/Input/CombatInputActions/IA_BasicLeftAttack.IA_BasicLeftAttack");
			IA_Right = LoadInputAction("/Game/Blueprints/Characters/Input/CombatInputActions/IA_BasicRightAttack.IA_BasicRightAttack");
			IA_Down = LoadInputAction("/Game/Blueprints/Characters/Input/CombatInputActions/IA_BasicDownAttack.IA_BasicDownAttack");
			IA_UP = LoadInputAction("/Game/Blueprints/Characters/Input/CombatInputActions/IA_BasicUpAttack.IA_BasicUpAttack");
			IA_Neutral = LoadInputAction("/Game/Blueprints/Characters/Input/CombatInputActions/IA_BasicAttack.IA_BasicAttack");
			break;
		case EActionAbilityType::Special:
			break;
	}
	
}

UInputAction* SAbilityActionMap::LoadInputAction(const FString& InputActionReferencePath)
{
	const TCHAR* RefPath = *InputActionReferencePath;

	if(UInputAction* Action = LoadObject<UInputAction>(nullptr, RefPath))
	{
		UE_LOG(LogTemp, Warning, TEXT("Loaded IA: %s from path: %s"), *Action->GetName(), *InputActionReferencePath);
		return Action;
	}
	
	UE_LOG(LogTemp, Error, TEXT("Could not load input action at: %s"), *InputActionReferencePath);
	return nullptr;
}

TSharedRef<SInputAbilityMappingWidget> SAbilityActionMap::CreateInputAbilityMappingWidget(UInputAction* Action, FName ImagePath)
{
	TSharedPtr<SInputAbilityMappingWidget> MappingWidget = SNew(SInputAbilityMappingWidget)
	.Action(Action) 
	.InputImage(ImagePath)
	.OnAbilitySelected(FSelectedAbility::CreateSP(this, &SAbilityActionMap::OnAbilitySelected));

	InputAbilityMappingWidgets.Add(MappingWidget);
	return MappingWidget.ToSharedRef();
}

void SAbilityActionMap::OnAbilitySelected(UCrashGameplayAbility* SelectedAbility, UInputAction* WidgetAction) const
{
	OnInputSelectedAbility.ExecuteIfBound(SelectedAbility, WidgetAction);
	FSlateApplication::Get().DismissAllMenus();
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
