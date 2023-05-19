// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SGameplayInterface.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "SInteractItemBase.generated.h"

UCLASS()
class ACTIONROGUELIKE_API ASInteractItemBase : public AActor, public ISGameplayInterface
{
private:
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASInteractItemBase();

	virtual void Interact_Implementation(APawn* InstigatorPawn) override;

protected:
	void HideAndCooldownPowerUp();

	UPROPERTY(EditAnywhere)
	USphereComponent* SphereComponent;
	
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* StaticMeshComponent;

	FTimerHandle TimerHandle_Use;

	void Use_TimeElapsed();
	void SetPowerupState(bool State);

	bool bIsInteractable = true;
};

