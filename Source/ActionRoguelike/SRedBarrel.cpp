// Fill out your copyright notice in the Description page of Project Settings.


#include "SRedBarrel.h"


// Sets default values
ASRedBarrel::ASRedBarrel()
{
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");
	StaticMeshComponent->SetSimulatePhysics(true);
	StaticMeshComponent->OnComponentHit.AddDynamic(this, &ThisClass::ApplyRadialForce);
	
	RootComponent = StaticMeshComponent;
	
	RadialForceComponent = CreateDefaultSubobject<URadialForceComponent>("RadialForceComponent");
	RadialForceComponent->SetAutoActivate(true);
	RadialForceComponent->Radius = 750.0f;
	RadialForceComponent->bImpulseVelChange = true;
	RadialForceComponent->AddCollisionChannelToAffect(ECC_WorldDynamic);
	RadialForceComponent->SetupAttachment(RootComponent);
}

void ASRedBarrel::ApplyRadialForce(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	RadialForceComponent->FireImpulse();

	FString DebugString = FString::Printf(TEXT("NAME: %s"), *GetNameSafe(OtherActor));
	DrawDebugString(GetWorld(), Hit.ImpactPoint, DebugString);
}
