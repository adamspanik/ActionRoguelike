// Fill out your copyright notice in the Description page of Project Settings.

#include "SMagicProjectile.h"

#include "SGameplayFunctionLibrary.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GAS/SActionComponent.h"

void ASMagicProjectile::OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor)
		return;

	if (OtherActor == GetInstigator())
		return;

	USActionComponent* ActionComponent = Cast<USActionComponent>(OtherActor->GetComponentByClass(USActionComponent::StaticClass()));
	if (ActionComponent && ActionComponent->ActiveGameplayTags.HasTag(ParryTag))
	{
		MovementComp->Velocity = -MovementComp->Velocity;
		SetInstigator(Cast<APawn>(OtherActor));
		return;
	}
	
	if (USGameplayFunctionLibrary::ApplyDirectionalDamage(GetInstigator(), OtherActor, DamageAmount, SweepResult))
	{
		if (ActionComponent)
		{
			ActionComponent->AddAction(GetInstigator(), BurningActionClass);
		}	
	}
	
	Destroy();	
}
