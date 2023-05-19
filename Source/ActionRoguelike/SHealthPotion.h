// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SInteractItemBase.h"
#include "GameFramework/Actor.h"
#include "SHealthPotion.generated.h"

UCLASS()
class ACTIONROGUELIKE_API ASHealthPotion : public ASInteractItemBase
{
public:
	virtual void Interact_Implementation(APawn* InstigatorPawn) override;
private:
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere)
	float HealthValue = 200.0f;

	UPROPERTY(EditAnywhere)
	int32 CreditCost = 15;
};
