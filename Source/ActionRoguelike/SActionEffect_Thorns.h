// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/SActionEffect.h"
#include "SActionEffect_Thorns.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API USActionEffect_Thorns : public USActionEffect
{
	GENERATED_BODY()

public:
	virtual void StartAction_Implementation(AActor* Instigator) override;

	virtual void StopAction_Implementation(AActor* Instigator) override;

	USActionEffect_Thorns();

protected:

	UPROPERTY(EditDefaultsOnly, Category="Thorns")
	float ReflectFraction = 0.5f;

	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta);	
};
