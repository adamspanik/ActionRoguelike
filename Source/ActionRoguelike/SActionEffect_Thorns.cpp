// Fill out your copyright notice in the Description page of Project Settings.


#include "SActionEffect_Thorns.h"

#include "SAttributeComponent.h"
#include "SGameplayFunctionLibrary.h"
#include "GAS/SActionComponent.h"

USActionEffect_Thorns::USActionEffect_Thorns()
{
	Duration = 0.0f;
	Period = 0.0f;
}

void USActionEffect_Thorns::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);
	
	USAttributeComponent* Attributes = USAttributeComponent::GetAttributes(GetOwningComponent()->GetOwner());
	if (Attributes)
	{
		Attributes->OnHealthChanged.AddDynamic(this, &USActionEffect_Thorns::OnHealthChanged);
	}
}

void USActionEffect_Thorns::StopAction_Implementation(AActor* Instigator)
{
	Super::StopAction_Implementation(Instigator);

	USAttributeComponent* Attributes = USAttributeComponent::GetAttributes(GetOwningComponent()->GetOwner());
	if (Attributes)
	{
		Attributes->OnHealthChanged.RemoveDynamic(this, &USActionEffect_Thorns::OnHealthChanged);
	}
}

void USActionEffect_Thorns::OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta)
{
	if (Delta > 0.0f)
		return;

	// Round to nearest to avoid 'ugly' damage numbers and tiny reflections
	int32 ReflectedAmount = FMath::RoundToInt(Delta * ReflectFraction);
	if (ReflectedAmount == 0)
		return;
	
	AActor* DamagedActor = OwningComp->GetOwner();
	if (!DamagedActor || DamagedActor == InstigatorActor)
		return;
	
	// Flip to positive, so we don't end up healing ourselves when passed into damage
	ReflectedAmount = FMath::Abs(ReflectedAmount);

	// Return damage sender...
	USGameplayFunctionLibrary::ApplyDamage(DamagedActor, InstigatorActor, ReflectedAmount);
}
