// Fill out your copyright notice in the Description page of Project Settings.


#include "SHealthPotion.h"

#include "SAttributeComponent.h"
#include "SCharacter.h"


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
	
	AttributeComponent->ApplyHealthChange(this, HealthValue);
	
	Super::Interact_Implementation(InstigatorPawn);
}
