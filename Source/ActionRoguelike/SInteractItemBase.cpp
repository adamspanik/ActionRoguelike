// Fill out your copyright notice in the Description page of Project Settings.

#include "SInteractItemBase.h"

#include "SCharacter.h"

ASInteractItemBase::ASInteractItemBase()
{
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
