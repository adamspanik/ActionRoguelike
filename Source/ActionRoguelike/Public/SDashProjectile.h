// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SProjectile.h"
#include "SDashProjectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;
class UParticleSystemComponent;

UCLASS()
class ACTIONROGUELIKE_API ASDashProjectile : public ASProjectile
{
	GENERATED_BODY()
	
public:
	ASDashProjectile();

protected:
	virtual void BeginPlay() override;
	void Attack_TimeElapsed();
	void Dash_TimeElapsed();

	FTimerHandle TimerHandle_Attack;
	FTimerHandle TimerHandle_Dash;


	UPROPERTY(EditAnywhere)
	UParticleSystem* ParticleSystem;
};
