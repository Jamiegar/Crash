// Fill out your copyright notice in the Description page of Project Settings.


#include "KillZone/RespawnAnchor.h"

#include "NiagaraFunctionLibrary.h"
#include "Characters/CrashCharacter.h"
#include "Components/BoxComponent.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GAS/Effects/Respawn/RespawnInvincibility.h"


// Sets default values
ARespawnAnchor::ARespawnAnchor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>("Root Component");
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("Respawn Pad Mesh");
	BoxCollision = CreateDefaultSubobject<UBoxComponent>("Box Collision");
	TimelineComponent = CreateDefaultSubobject<UTimelineComponent>("Timeline Component");

	StaticMesh->SetupAttachment(RootComponent);
	BoxCollision->SetupAttachment(RootComponent);
	
	static ConstructorHelpers::FObjectFinder<UCurveFloat> DefaultCurveFlyIn
		(TEXT("/Script/Engine.CurveFloat'/Game/Blueprints/Characters/Respawn/RespawnFlyIn.RespawnFlyIn'"));

	FlyInCurve = DefaultCurveFlyIn.Object;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> DefaultRespawnPadMesh
		(TEXT("/Script/Engine.StaticMesh'/Game/Assets/PolygonSciFiSpace/Meshes/Props/SM_Prop_MapTable_01.SM_Prop_MapTable_01'"));

	StaticMesh->SetStaticMesh(DefaultRespawnPadMesh.Object);

	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> DefaultDestroySystem
		(TEXT("/Script/Niagara.NiagaraSystem'/Game/Assets/Vefects/Flipbook_VFX/Shared/Particles/Sparkles/NS_Sparkles_01.NS_Sparkles_01'"));

	DestroyedEffect = DefaultDestroySystem.Object;

	StaticMesh->SetWorldScale3D(FVector(0.5, 0.5, 0.5));
	StaticMesh->SetRelativeLocation(FVector(0, 0, -175));

	BoxCollision->SetBoxExtent(FVector(60, 60, 60));
	BoxCollision->OnComponentEndOverlap.AddUniqueDynamic(this, &ARespawnAnchor::OnEndOverlap);
}

void ARespawnAnchor::RespawnCharacter(ACrashCharacter* Character, FVector TargetPosition)
{
	TargetLocation = TargetPosition;
	StartLocation = TargetLocation + StartLocationOffset;
	CrashCharacter = Character;
	
	SetActorLocation(StartLocation);
	CrashCharacter->SetActorLocation(GetActorLocation());
	CrashCharacter->AttachToActor(this, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	CrashCharacter->GetCharacterMovement()->SetMovementMode(MOVE_Falling);
	CrashCharacter->bEndKnockback = false;
	CrashCharacter->ApplyEffectToCrashCharacter(URespawnInvincibility::StaticClass());

	FOnTimelineEventStatic OnTimelineFinish;
	FOnTimelineFloat TimelineFloat;
	TimelineComponent->SetLooping(false);

	TimelineFloat.BindUFunction(this, FName("TimelineUpdate"));
	TimelineComponent->AddInterpFloat(FlyInCurve, TimelineFloat);

	OnTimelineFinish.BindUFunction(this, "OnTimelineFinished");
	TimelineComponent->SetTimelineFinishedFunc(OnTimelineFinish);

	TimelineComponent->PlayFromStart();
}

void ARespawnAnchor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(TimelineComponent)
		TimelineComponent->TickComponent(DeltaTime, LEVELTICK_TimeOnly, nullptr);
}

void ARespawnAnchor::TimelineUpdate(float InterpValue)
{
	const FVector NewLocation = FMath::Lerp(StartLocation, TargetLocation, InterpValue);
	SetActorLocation(NewLocation);
}

void ARespawnAnchor::OnTimelineFinished()
{
	GetWorldTimerManager().SetTimer(LifeTimeHandle, this, &ARespawnAnchor::OnLifeTimeTimerEnd, LifeTimeLength);
}

void ARespawnAnchor::OnLifeTimeTimerEnd()
{
	Destroy();
}

void ARespawnAnchor::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if(Cast<ACrashCharacter>(OtherActor))
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, DestroyedEffect, StaticMesh->GetComponentLocation(), FRotator());
		Destroy();
	}
}




