// Fill out your copyright notice in the Description page of Project Settings.


#include "SAICharacter.h"

#include "AIController.h"
#include "BrainComponent.h"
#include "ActionRoguelike/SAttributeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/PawnSensingComponent.h"

ASAICharacter::ASAICharacter()
{
	PawnSensingComponent = CreateDefaultSubobject<UPawnSensingComponent>("PawnSensingComponent");
	AttributeComponent = CreateDefaultSubobject<USAttributeComponent>("AttributeComponent");

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;	
}

void ASAICharacter::SetTargetActor(AActor* Targer)
{
	AAIController* AIController = Cast<AAIController>(GetController());
	if (!Controller)
		return;

	UBlackboardComponent* BlackboardComponent = AIController->GetBlackboardComponent();
	BlackboardComponent->SetValueAsObject("TargetActor", Targer);
}

void ASAICharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	PawnSensingComponent->OnSeePawn.AddDynamic(this, &ThisClass::OnPawnSeen);
	AttributeComponent->OnHealthChanged.AddDynamic(this, &ThisClass::OnHealthChanged);
}

void ASAICharacter::OnPawnSeen(APawn* Pawn)
{
	SetTargetActor(Pawn);
	DrawDebugString(GetWorld(), GetActorLocation(), "PLAYER SPOTTED", nullptr, FColor::White, 4.0f, true);
}

void ASAICharacter::OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta)
{
	if (Delta < 0.0f)
	{
		GetMesh()->SetScalarParameterValueOnMaterials("HitFlashTime", GetWorld()->TimeSeconds);
		
		if (InstigatorActor != this)
		{
			SetTargetActor(InstigatorActor);
		}
		
		if (NewHealth <= 0.0f)
		{
			AAIController* AIController = Cast<AAIController>(GetController());
			if (AIController)
			{
				AIController->GetBrainComponent()->StopLogic("Killed");
			}

			GetMesh()->SetAllBodiesSimulatePhysics(true);
			GetMesh()->SetCollisionProfileName("Ragdoll");
			
			SetLifeSpan(10.0f);
		}
	}
}
