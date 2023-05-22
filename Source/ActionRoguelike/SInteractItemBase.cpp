// Fill out your copyright notice in the Description page of Project Settings.

#include "SInteractItemBase.h"

#include "SCharacter.h"

ASInteractItemBase::ASInteractItemBase()
{
	SphereComponent = CreateDefaultSubobject<USphereComponent>("SphereComponent");
	SphereComponent->SetCollisionProfileName("PowerUp");
	RootComponent = SphereComponent;
	
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");
	StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	StaticMeshComponent->SetupAttachment(SphereComponent);

	SetReplicates(true);
}

void ASInteractItemBase::Interact_Implementation(APawn* InstigatorPawn)
{
	if (!bIsInteractable)
		return;

	ISGameplayInterface::Interact_Implementation(InstigatorPawn);
}

void ASInteractItemBase::HideAndCooldownPowerUp()
{
	SetPowerupState(false);
	
	GetWorldTimerManager().SetTimer(TimerHandle_Use, this, &ASInteractItemBase::Use_TimeElapsed, 10.0f);
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
