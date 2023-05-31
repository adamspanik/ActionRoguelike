// Fill out your copyright notice in the Description page of Project Settings.


#include "SAbilityItem.h"


#include "SCharacter.h"
#include "GAS/SActionComponent.h"

void ASAbilityItem::Interact_Implementation(APawn* InstigatorPawn)
{
	const ASCharacter* Character = Cast<ASCharacter>(InstigatorPawn);
	if (!Character)
		return;

	USActionComponent* ActionComponent = Character->GetActionComponent();
	if (!ActionComponent)
		return;

	if(ActionComponent->AddAction(InstigatorPawn, ActionToGrant))
		HideAndCooldownPowerUp();
		
	Super::Interact_Implementation(InstigatorPawn);
}
