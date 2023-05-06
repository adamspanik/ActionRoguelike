// Fill out your copyright notice in the Description page of Project Settings.

#include "SInteractItemBase.h"

#include "SCharacter.h"


// Sets default values
ASInteractItemBase::ASInteractItemBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");
	RootComponent = StaticMeshComponent;
}

void ASInteractItemBase::Interact_Implementation(APawn* InstigatorPawn)
{
	if (!bIsInteractable)
		return;
	
	SetPowerupState(false);
	
	GetWorldTimerManager().SetTimer(TimerHandle_Use, this, &ASInteractItemBase::Use_TimeElapsed, 10.0f);

	ISGameplayInterface::Interact_Implementation(InstigatorPawn);
}

// Called when the game starts or when spawned
void ASInteractItemBase::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ASInteractItemBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASInteractItemBase::Use_TimeElapsed()
{
	GetWorldTimerManager().ClearTimer(TimerHandle_Use);
	SetPowerupState(true);
}

void ASInteractItemBase::SetPowerupState(bool State)
{
	bIsInteractable = State;
	SetActorEnableCollision(State);
	RootComponent->SetVisibility(State, true);
}

