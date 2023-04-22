// Fill out your copyright notice in the Description page of Project Settings.


#include "SRedBarrel.h"


// Sets default values
ASRedBarrel::ASRedBarrel()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");
	StaticMeshComponent->SetSimulatePhysics(true);
	StaticMeshComponent->OnComponentHit.AddDynamic(this, &ThisClass::ApplyRadialForce);
	
	RootComponent = StaticMeshComponent;
	
	RadialForceComponent = CreateDefaultSubobject<URadialForceComponent>("RadialForceComponent");
	RadialForceComponent->Radius = 600.0f;
	RadialForceComponent->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ASRedBarrel::BeginPlay()
{
	Super::BeginPlay();
}

void ASRedBarrel::ApplyRadialForce(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	RadialForceComponent->FireImpulse();
}

// Called every frame
void ASRedBarrel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}





