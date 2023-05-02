// Fill out your copyright notice in the Description page of Project Settings.

#include "SDashProjectile.h"

#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

ASDashProjectile::ASDashProjectile()
{
}

void ASDashProjectile::BeginPlay()
{
	Super::BeginPlay();

	GetWorldTimerManager().SetTimer(TimerHandle_Attack, this, &ASDashProjectile::Attack_TimeElapsed,  0.2f);
}

void ASDashProjectile::Attack_TimeElapsed()
{
	GetWorldTimerManager().ClearTimer(TimerHandle_Attack);
	
	MovementComp->StopMovementImmediately();
	EffectComp->DeactivateSystem();
	SetActorEnableCollision(false);
	UGameplayStatics::SpawnEmitterAtLocation(this, ParticleSystem, GetActorLocation());
	GetWorldTimerManager().SetTimer(TimerHandle_Dash, this, &ASDashProjectile::Dash_TimeElapsed,  0.2f);
}

void ASDashProjectile::Dash_TimeElapsed()
{
	AActor* ActorToTeleport = GetInstigator();
	if (ensure(ActorToTeleport))
	{
		ActorToTeleport->TeleportTo(GetActorLocation(), ActorToTeleport->GetActorRotation());
	}

	Destroy();
}

