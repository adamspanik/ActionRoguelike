// Fill out your copyright notice in the Description page of Project Settings.


#include "SGameplayFunctionLibrary.h"

#include "SAttributeComponent.h"

bool USGameplayFunctionLibrary::ApplyDamage(AActor* DamageCauser, AActor* TargetActor, float DamageAmount)
{
	USAttributeComponent* AttributeComponent = USAttributeComponent::GetAttributes(TargetActor);
	if (!AttributeComponent)
		return false;

	return AttributeComponent->ApplyHealthChange(DamageCauser, DamageAmount);
}

bool USGameplayFunctionLibrary::ApplyDirectionalDamage(AActor* DamageCauser, AActor* TargetActor, float DamageAmount, const FHitResult& OutHitResult)
{
	if (!ApplyDamage(DamageCauser, TargetActor, DamageAmount))
		return false;

	UPrimitiveComponent* HitComp = OutHitResult.GetComponent();
	if (!HitComp || !HitComp->IsSimulatingPhysics(OutHitResult.BoneName))
		return false;
	
	HitComp->AddImpulseAtLocation(-OutHitResult.ImpactNormal * 1000000.0f, OutHitResult.ImpactPoint, OutHitResult.BoneName);

	return true;
}
