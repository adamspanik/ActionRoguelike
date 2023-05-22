// Fill out your copyright notice in the Description page of Project Settings.
#include "SInteractionComponent.h"
#include "SGameplayInterface.h"
#include "SWorldUserWidget.h"
#include "Blueprint/UserWidget.h"

static TAutoConsoleVariable<bool> CVarDebugDrawInteraction(TEXT("su.DebugDrawInteraction"), false, TEXT("Enable draw debug for interact component"), ECVF_Cheat);

USInteractionComponent::USInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void USInteractionComponent::BeginPlay()
{
	Super::BeginPlay();
}

void USInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	APawn* MyPawn = Cast<APawn>(GetOwner());
	if (MyPawn->IsLocallyControlled())
	{
		FindBestInteractable();
	}
}

void USInteractionComponent::FindBestInteractable()
{
	bool bDebugDraw = CVarDebugDrawInteraction.GetValueOnGameThread();
	
	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(CollisionChannel);

	AActor* Owner = GetOwner();
	FVector EyeLocation;
	FRotator EyeRotation;
	Owner->GetActorEyesViewPoint(EyeLocation, EyeRotation);
	
	FVector End = EyeLocation + EyeRotation.Vector() * TraceDistance;

	// FHitResult OutHit;
	// bool BlockingHit = GetWorld()->LineTraceSingleByObjectType(OutHit, EyeLocation, End, ObjectQueryParams);
	
	TArray<FHitResult> OutHits;
	FCollisionShape CollisionShape;
	CollisionShape.SetSphere(TraceRadius);
	FQuat Rot = FQuat::Identity;
	bool BlockingHit = GetWorld()->SweepMultiByObjectType(OutHits, EyeLocation, End, Rot, ObjectQueryParams, CollisionShape);

	FColor Color = BlockingHit ? FColor::Green : FColor::Red;

	FocusedActor = nullptr;
	for (FHitResult OutHit : OutHits)
	{
		if (bDebugDraw)
			DrawDebugSphere(GetWorld(), OutHit.ImpactPoint, TraceRadius, 16, Color, false,0.0f, 0, 2.0f);
		
		AActor* HitActor = OutHit.GetActor();
		if(HitActor && HitActor->Implements<USGameplayInterface>())
		{
			FocusedActor = HitActor;
			break;
		}
	}

	if (FocusedActor)
	{
		if (!DefaultWidgetInstance && ensure(DefaultWidgetClass))
		{
			DefaultWidgetInstance = CreateWidget<USWorldUserWidget>(GetWorld(), DefaultWidgetClass);	
		}

		if (DefaultWidgetInstance)
		{
			DefaultWidgetInstance->AttachedActor = FocusedActor;

			if (!DefaultWidgetInstance->IsInViewport())
			{
				DefaultWidgetInstance->AddToViewport();
			}
		}
	}
	else
	{
		if (DefaultWidgetInstance)
		{
			DefaultWidgetInstance->RemoveFromParent();
		}
	}
	
	if (bDebugDraw)
		DrawDebugLine(GetWorld(), EyeLocation, End, Color, false, 0.0f, 0, 2.0f);

}

void USInteractionComponent::PrimaryInteract()
{
	ServerInteract(FocusedActor);
}

void USInteractionComponent::ServerInteract_Implementation(AActor* InFocus)
{
	if (!InFocus)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, "No InFocus to interact");
		return;
	}
		
	
	APawn* Pawn = Cast<APawn>(GetOwner());
	ISGameplayInterface::Execute_Interact(InFocus, Pawn);
}