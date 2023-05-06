// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTN_RangedAttack.h"

#include "AIController.h"
#include "ActionRoguelike/SAttributeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "GameFramework/Character.h"

EBTNodeResult::Type UBTTN_RangedAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AiController = OwnerComp.GetAIOwner();
	if (!ensure(AiController))
		return EBTNodeResult::Failed;

	ACharacter* MyCharacter = Cast<ACharacter>(AiController->GetPawn());
	if (!MyCharacter)
		return EBTNodeResult::Failed;

	FVector MuzzleLocation = MyCharacter->GetMesh()->GetSocketLocation("Muzzle_01");

	AActor* TargetActor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("TargetActor"));
	if (!TargetActor)
		return EBTNodeResult::Failed;

	if (!USAttributeComponent::IsActorAlive(TargetActor))
	{
		return EBTNodeResult::Failed;
	}
	
	// target - origin
	FVector Direction = TargetActor->GetActorLocation() - MuzzleLocation;
	FRotator MuzzleRotation = Direction.Rotation();

	MuzzleRotation.Pitch += FMath::RandRange(0.0f, MaxBulletSpread);
	MuzzleRotation.Yaw += FMath::RandRange(-MaxBulletSpread, MaxBulletSpread);
	
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParameters.Instigator = MyCharacter;

	AActor* NewProjectile = GetWorld()->SpawnActor<AActor>(ProjectileClass, MuzzleLocation, MuzzleRotation, SpawnParameters);

	return NewProjectile ? EBTNodeResult::Succeeded : EBTNodeResult::Failed;
	// return Super::ExecuteTask(OwnerComp, NodeMemory);
}
