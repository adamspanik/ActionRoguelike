// Fill out your copyright notice in the Description page of Project Settings.


#include "SActionComponent.h"

#include "SAction.h"

USActionComponent::USActionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	SetIsReplicatedByDefault(true);
}

void USActionComponent::BeginPlay()
{
	Super::BeginPlay();

	for (const TSubclassOf<USAction> Action : DefaultsActions)
	{
		AddAction(GetOwner(), Action);
	}
}

void USActionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FString DebugMessage = GetNameSafe(GetOwner()) + " : " + ActiveGameplayTags.ToStringSimple();
	GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::White, DebugMessage);
}

bool USActionComponent::GotAction(AActor* Instigator, TSubclassOf<USAction> ActionClass)
{
	if (!ensure(ActionClass))
		return false;

	for (USAction* Action : Actions)
	{
		if (Action->IsA(ActionClass))
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, "GOT ABILITY ALREADY!");
			return true;
		}
	}

	return false;
	//return Actions.Contains(ActionClass.GetDefaultObject()) ? true : false;
}

bool USActionComponent::AddAction(AActor* Instigator, TSubclassOf<USAction> ActionClass)
{
	if (!ensure(ActionClass))
		return false;

	if (GotAction(Instigator, ActionClass))
		return false;

	USAction* NewAction = NewObject<USAction>(this, ActionClass);
	if (ensure(NewAction))
	{
		Actions.Add(NewAction);

		if (NewAction->bAutoStart && NewAction->CanStartAction(Instigator))
		{
			NewAction->StartAction(Instigator);
		}
	}

	return true;
}

bool USActionComponent::StartActionByName(AActor* Instigator, FName ActionName)
{
	for (USAction* Action : Actions)
	{
		if (Action && Action->ActionName == ActionName)
		{
			if (!Action->CanStartAction(Instigator))
			{
				FString DebugMessage = FString::Printf(TEXT("Failed to start action %s"), *ActionName.ToString());
				GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, DebugMessage);
				continue;
			}

			if (!GetOwner()->HasAuthority())
			{
				ServerStartAction(Instigator, ActionName);	
			}
				
			Action->StartAction(Instigator);
			return true;
		}
	}

	return false;
}

bool USActionComponent::StopActionByName(AActor* Instigator, FName ActionName)
{
	for (USAction* Action : Actions)
	{
		if (Action && Action->ActionName == ActionName)
		{
			if (!Action->IsRunning())
			{
				FString DebugMessage = FString::Printf(TEXT("Failed to stop action %s"), *ActionName.ToString());
				GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, DebugMessage);
				continue;
			}
			
			Action->StopAction(Instigator);
			return true;
		}
	}

	return false;
}

void USActionComponent::RemoveAction(USActionEffect* ActionToRemove)
{
	if (!ensure(ActionToRemove && !ActionToRemove->IsRunning()))
		return;
	
	Actions.Remove(ActionToRemove);
}

void USActionComponent::ServerStartAction_Implementation(AActor* Instigator, FName ActionName)
{
	StartActionByName(Instigator, ActionName);
}
