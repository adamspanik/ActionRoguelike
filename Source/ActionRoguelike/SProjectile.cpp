// Fill out your copyright notice in the Description page of Project Settings.


#include "SProjectile.h"

#include "SAttributeComponent.h"
#include "SGameplayFunctionLibrary.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"


ASProjectile::ASProjectile()
{
	SphereComp = CreateDefaultSubobject<USphereComponent>("SphereComp");
	SphereComp->SetCollisionObjectType(ECC_WorldDynamic);
	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnActorOverlap);
	SphereComp->SetCollisionProfileName("Projectile");
	RootComponent = SphereComp;

	EffectComp = CreateDefaultSubobject<UParticleSystemComponent>("EffectComp");
	EffectComp->SetupAttachment(SphereComp);

	MovementComp = CreateDefaultSubobject<UProjectileMovementComponent>("MovementComp");
	MovementComp->InitialSpeed = 1000.0f;
	MovementComp->bRotationFollowsVelocity = true;
	MovementComp->bInitialVelocityInLocalSpace = true;
}


void ASProjectile::OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor)
		return;

	if (OtherActor == GetInstigator())
		return;

	/*if(!USGameplayFunctionLibrary::ApplyDirectionalDamage(GetInstigator(), OtherActor, -DamageAmount, SweepResult))
		return;*/

	USGameplayFunctionLibrary::ApplyDirectionalDamage(GetInstigator(), OtherActor, -DamageAmount, SweepResult);

	Destroy();	
	// USAttributeComponent* AttributeComponent = Cast<USAttributeComponent>(OtherActor->GetComponentByClass(USAttributeComponent::StaticClass()));
	//
	// if (!AttributeComponent)
	// 	return;
	//
	// AttributeComponent->ApplyHealthChange(GetInstigator(),-20.0f);
	// Destroy();	
}
