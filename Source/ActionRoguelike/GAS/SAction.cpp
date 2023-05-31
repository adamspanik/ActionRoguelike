// Fill out your copyright notice in the Description page of Project Settings.


#include "SAction.h"

#include "SActionComponent.h"
#include "ActionRoguelike/SAttributeComponent.h"
#include "ActionRoguelike/SCharacter.h"

bool USAction::CanStartAction_Implementation(AActor* Instigator)
{
	if (IsRunning())
		return false;
	
	USActionComponent* ActionComponent = GetOwningComponent();
	if (ActionComponent->ActiveGameplayTags.HasAny(BlockedTags))
		return false;

	/*const ASCharacter* CharacterInstigator = Cast<ASCharacter>(Instigator);
	if (CharacterInstigator && CharacterInstigator->GetAttributeComponent()->GetRage() < RageCost)
		return false;*/

	return true;
}

void USAction::StartAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp, Warning, TEXT("Running: %s"), *GetNameSafe(this));
	
	USActionComponent* ActionComponent = GetOwningComponent();
	ActionComponent->ActiveGameplayTags.AppendTags(GrantsTags);

	/*const ASCharacter* CharacterInstigator = Cast<ASCharacter>(Instigator);
	if (CharacterInstigator)
	{
		CharacterInstigator->GetAttributeComponent()->ApplyRageChange(Instigator, -RageCost);
	}*/
	
	bIsRunning = true;
}

void USAction::StopAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp, Warning, TEXT("Stopped: %s"), *GetNameSafe(this));

	ensureAlways(bIsRunning);
	
	USActionComponent* ActionComponent = GetOwningComponent();
	ActionComponent->ActiveGameplayTags.RemoveTags(GrantsTags);

	bIsRunning = false;
}

UWorld* USAction::GetWorld() const
{
	UActorComponent* ActorComponent = Cast<UActorComponent>(GetOuter());
	if (!ActorComponent)
		return nullptr;

	return ActorComponent->GetWorld();
}

USActionComponent* USAction::GetOwningComponent() const
{
	return Cast<USActionComponent>(GetOuter());
}
