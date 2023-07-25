// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityMapping/InputAbilityMappingWidget.h"
#include "ClassViewerModule.h"
#include "SlateOptMacros.h"
#include "Abilities/GameplayAbility.h"
#include "Filter/FClassPickerViewFilter.h"


BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SInputAbilityMappingWidget::Construct(const FArguments& InArgs)
{
	
	ChildSlot
	[
		SNew(SComboButton)
		.ButtonContent()
		[
			SNew(SImage)
			.Image(new FSlateDynamicImageBrush(InArgs._InputImage, FVector2D(64, 64)))
		]
		.OnGetMenuContent(this, &SInputAbilityMappingWidget::OnMappingButtonClicked)
		
	];
	
}

TSharedRef<SWidget> SInputAbilityMappingWidget::OnMappingButtonClicked()
{
	FClassViewerModule& ClassViewerModule = FModuleManager::LoadModuleChecked<FClassViewerModule>("ClassViewer");

	const TSharedRef<FClassPickerViewFilter> ClassFilter = MakeShared<FClassPickerViewFilter>();
	ClassFilter.Get().AllowedClass.Add(UGameplayAbility::StaticClass());  

	FClassViewerInitializationOptions Options;
	Options.ClassFilters.Add(ClassFilter);
	Options.Mode = EClassViewerMode::Type::ClassPicker;

	const TSharedRef<SWidget> ClassViewer = ClassViewerModule.CreateClassViewer(Options, FOnClassPicked::CreateRaw(this, &SInputAbilityMappingWidget::OnClassPicked));
	
	return
		SNew(SBox)
		.HeightOverride(300)
		.WidthOverride(300)
		[
			ClassViewer
		];
}

void SInputAbilityMappingWidget::OnClassPicked(UClass* SelectedClass)
{
	UE_LOG(LogTemp, Warning, TEXT("Selected CLass = %s"), *SelectedClass->GetName())
	
}


END_SLATE_FUNCTION_BUILD_OPTIMIZATION
