// Fill out your copyright notice in the Description page of Project Settings.

#include "BTService_CheckLowHealth.h"

#include "AIController.h"
#include "ActionRoguelike/SAttributeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

void UBTService_CheckLowHealth::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	
	UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent();
	if (!ensure(BlackboardComponent))
		return;
	
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!ensure(AIController))
		return;
	
	APawn* AIPawn = AIController->GetPawn();
	if (!ensure(AIPawn))
		return;

	USAttributeComponent* AttributeComponent = USAttributeComponent::GetAttributes(AIPawn);
	if (AttributeComponent->GetHealth() < HealthThreshold)
	{
		BlackboardComponent->SetValueAsBool(IsLowHealthKey.SelectedKeyName, true);
	}
	else
	{
		BlackboardComponent->SetValueAsBool(IsLowHealthKey.SelectedKeyName, false);
	}
}
