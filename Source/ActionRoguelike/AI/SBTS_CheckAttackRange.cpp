// Fill out your copyright notice in the Description page of Project Settings.


#include "SBTS_CheckAttackRange.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

void USBTS_CheckAttackRange::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	
	UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent();
	if (!ensure(BlackboardComponent))
		return;

	AActor* TargetActor = Cast<AActor>(BlackboardComponent->GetValueAsObject(TargetActorKey.SelectedKeyName));
	if (!TargetActor)
		return;

	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!ensure(AIController))
		return;
	
	APawn* AIPawn = AIController->GetPawn();
	if (!ensure(AIPawn))
		return;

	double Distance = FVector::Distance(TargetActor->GetActorLocation(), AIPawn->GetActorLocation());
	
	bool bWithinRange = Distance < 2000.0f;
	
	BlackboardComponent->SetValueAsBool(AttackRangeKey.SelectedKeyName, bWithinRange && AIController->LineOfSightTo(TargetActor));
}
