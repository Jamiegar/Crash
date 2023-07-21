// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityMapping/InputAbilityMappingWidget.h"
#include "ClassViewerFilter.h"
#include "ClassViewerModule.h"
#include "SlateOptMacros.h"
#include "Abilities/GameplayAbility.h"

class FGameplayAbilityClassFilter : public IClassViewerFilter
{
public:
	TSet<const UClass*> AllowedClass;
	
	virtual bool IsClassAllowed(const FClassViewerInitializationOptions& InInitOptions, const UClass* InClass, TSharedRef<FClassViewerFilterFuncs> InFilterFuncs) override
	{
		if(InFilterFuncs->IfInChildOfClassesSet(AllowedClass, InClass) != EFilterReturn::Failed)
		{
			return true;
		}
		return false;
	}

	virtual bool IsUnloadedClassAllowed(const FClassViewerInitializationOptions& InInitOptions,const TSharedRef<const IUnloadedBlueprintData> InUnloadedClassData,
		TSharedRef<FClassViewerFilterFuncs> InFilterFuncs) override
	{
		return InFilterFuncs->IfInChildOfClassesSet(AllowedClass, InUnloadedClassData) != EFilterReturn::Failed;
	}
};

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

	const TSharedRef<FGameplayAbilityClassFilter> ClassFilter = MakeShared<FGameplayAbilityClassFilter>();
	ClassFilter.Get().AllowedClass.Add(UGameplayAbility::StaticClass());  

	FClassViewerInitializationOptions Options;
	Options.ClassFilters.Add(ClassFilter);

	const TSharedRef<SWidget> ClassViewer = ClassViewerModule.CreateClassViewer(Options, FOnClassPicked::CreateRaw(this, &SInputAbilityMappingWidget::OnClassPicked));
	
	return
		SNew(SBox)
		.HeightOverride(300)
		.WidthOverride(300)
		[
			ClassViewer
		];
}

void SInputAbilityMappingWidget::OnSelctedAsset(const FAssetData& Data)
{
}

void SInputAbilityMappingWidget::OnClassPicked(UClass* PickedClass)
{
}




END_SLATE_FUNCTION_BUILD_OPTIMIZATION
