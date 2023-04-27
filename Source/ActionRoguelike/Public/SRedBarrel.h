// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Actor.h"
#include "PhysicsEngine/RadialForceComponent.h"
#include "SRedBarrel.generated.h"

UCLASS()
class ACTIONROGUELIKE_API ASRedBarrel : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASRedBarrel();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UFUNCTION()
	virtual void ApplyRadialForce(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* StaticMeshComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	URadialForceComponent* RadialForceComponent;
	
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
};
