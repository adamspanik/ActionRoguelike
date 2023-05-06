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
	
	AttributeComponent->ApplyHealthChange(HealthValue);
	
	Super::Interact_Implementation(InstigatorPawn);
}

// Sets default values
ASHealthPotion::ASHealthPotion()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ASHealthPotion::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASHealthPotion::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

