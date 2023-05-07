// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_HealSelf.h"

#include "AIController.h"
#include "ActionRoguelike/SAttributeComponent.h"
#include "GameFramework/Character.h"


EBTNodeResult::Type UBTTask_HealSelf::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!ensure(AIController))
		return EBTNodeResult::Failed;;
	
	APawn* AIPawn = AIController->GetPawn();
	if (!ensure(AIPawn))
		return EBTNodeResult::Failed;;

	USAttributeComponent* AttributeComponent = USAttributeComponent::GetAttributes(AIPawn);
	if (!AttributeComponent)
		return EBTNodeResult::Failed;

	return AttributeComponent->ApplyHealthChange(AIPawn, HealAmount) ? EBTNodeResult::Succeeded : EBTNodeResult::Failed;
}

