// Fill out your copyright notice in the Description page of Project Settings.


#include "SInteractionComponent.h"
#include "SGameplayInterface.h"


// Sets default values for this component's properties
USInteractionComponent::USInteractionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void USInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void USInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                           FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void USInteractionComponent::PrimaryInteract()
{
	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);

	AActor* Owner = GetOwner();
	FVector EyeLocation;
	FRotator EyeRotation;
	Owner->GetActorEyesViewPoint(EyeLocation, EyeRotation);
	
	
	FVector End = EyeLocation + EyeRotation.Vector() * 1000;

	// FHitResult OutHit;
	// bool BlockingHit = GetWorld()->LineTraceSingleByObjectType(OutHit, EyeLocation, End, ObjectQueryParams);

	float SphereRadius = 30.0f;
	TArray<FHitResult> OutHits;
	FCollisionShape CollisionShape;
	CollisionShape.SetSphere(SphereRadius);
	FQuat Rot = FQuat::Identity;
	bool BlockingHit = GetWorld()->SweepMultiByObjectType(OutHits, EyeLocation, End, Rot, ObjectQueryParams, CollisionShape);

	FColor Color = BlockingHit ? FColor::Green : FColor::Red;
	
	for (FHitResult OutHit : OutHits)
	{
		AActor* HitActor = OutHit.GetActor();
		if(HitActor && HitActor->Implements<USGameplayInterface>())
		{
			APawn* Pawn = Cast<APawn>(Owner);
			ISGameplayInterface::Execute_Interact(HitActor, Pawn);
			break;
		}

		DrawDebugSphere(GetWorld(), OutHit.ImpactPoint, SphereRadius, 16, Color, false,2.0f, 0, 2.0f);
	}
	DrawDebugLine(GetWorld(), EyeLocation, End, Color, false, 2.0f, 0, 2.0f);
	
}

