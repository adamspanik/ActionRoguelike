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

public:
	// Sets default values for this actor's properties
	ASHealthPotion();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	float HealthValue = 15.0f;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
