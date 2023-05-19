// Fill out your copyright notice in the Description page of Project Settings.


#include "SHealthPotion.h"

#include "SAttributeComponent.h"
#include "SCharacter.h"
#include "SPlayerState.h"


void ASHealthPotion::Interact_Implementation(APawn* InstigatorPawn)
{
	const ASCharacter* Character = Cast<ASCharacter>(InstigatorPawn);
	if (!Character)
		return;

	USAttributeComponent* AttributeComponent = Character->GetAttributeComponent();
	if (!AttributeComponent)
		return;

	if (AttributeComponent->GetHealth() == AttributeComponent->GetMaxHealth())
		return;

	ASPlayerState* PlayerState = InstigatorPawn->GetPlayerState<ASPlayerState>();
	if (!PlayerState)
		return;

	if (PlayerState->RemoveCredits(CreditCost) && AttributeComponent->ApplyHealthChange(this, HealthValue))
		HideAndCooldownPowerUp();
	
	
	Super::Interact_Implementation(InstigatorPawn);
}
