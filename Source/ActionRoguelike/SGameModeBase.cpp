// Fill out your copyright notice in the Description page of Project Settings.
#include "SGameModeBase.h"

#include "EngineUtils.h"
#include "SAttributeComponent.h"
#include "SCharacter.h"
#include "AI/SAICharacter.h"
#include "EnvironmentQuery/EnvQueryManager.h"

static TAutoConsoleVariable<bool> CVarSpawnBots(TEXT("su.SpawnBots"), true, TEXT("Enable spawning bots via timer"), ECVF_Cheat);

void ASGameModeBase::StartPlay()
{
	Super::StartPlay();

	GetWorldTimerManager().SetTimer(TimerHandle_SpawnBots, this, &ThisClass::SpawnBotTimeElapsed, SpawnTimerInterval, true);
}

void ASGameModeBase::SpawnBotTimeElapsed()
{
	if (!CVarSpawnBots.GetValueOnGameThread())
	{
		UE_LOG(LogTemp, Warning, TEXT("BOT SPAWNING DISABLED VIA CVAR 'CVarSpawnBots'"));
		return;
	}
	
	int32 NumberOfAliveBots = 0;
	for (ASAICharacter* Bot : TActorRange<ASAICharacter>(GetWorld()))
	{
		USAttributeComponent* AttributeComponent = USAttributeComponent::GetAttributes(Bot);
		if (ensure(AttributeComponent) && AttributeComponent->IsAlive())
		{
			NumberOfAliveBots++;
		}
	}

	if (DifficultyCurve)
	{
		MaxBotCount = DifficultyCurve->GetFloatValue(GetWorld()->TimeSeconds);
	}

	if (NumberOfAliveBots >= MaxBotCount)
	{
		UE_LOG(LogTemp, Warning, TEXT("MAX CAPACITY OF BOTS SKIPPING SPAWN!"));
		return;
	}
	
	UEnvQueryInstanceBlueprintWrapper* QueryInstanceBlueprintWrapper = UEnvQueryManager::RunEQSQuery(this, SpawnBotQuery, this, EEnvQueryRunMode::RandomBest5Pct, nullptr);

	if (ensure(QueryInstanceBlueprintWrapper))
	{
		QueryInstanceBlueprintWrapper->GetOnQueryFinishedEvent().AddDynamic(this, &ThisClass::OnQueryCompleted);
	}
}

void ASGameModeBase::OnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus)
{
	if (QueryStatus != EEnvQueryStatus::Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("SPAWN FAILED OnQueryCompleted"));
		return;
	}
	
	TArray<FVector> Locations = QueryInstance->GetResultsAsLocations();

	if (Locations.IsValidIndex(0))
	{
		GetWorld()->SpawnActor<AActor>(MinionClass, Locations[0], FRotator::ZeroRotator);

		DrawDebugSphere(GetWorld(), Locations[0], 50.0f, 20, FColor::Blue, false,60.0f);
	}
}

void ASGameModeBase::KillAll()
{
	for (ASAICharacter* Bot : TActorRange<ASAICharacter>(GetWorld()))
	{
		USAttributeComponent* AttributeComponent = USAttributeComponent::GetAttributes(Bot);
		if (ensure(AttributeComponent) && AttributeComponent->IsAlive())
		{
			AttributeComponent->Kill(this);
		}
	}
}

void ASGameModeBase::OnActorKilled(AActor* Victim, AActor* Killer)
{
	UE_LOG(LogTemp, Warning, TEXT("OnActorKilled Victim: %s, Killer: %s"), *GetNameSafe(Victim), *GetNameSafe(Killer));
	
	ASCharacter* Player = Cast<ASCharacter>(Victim);
	if (!Player)
		return;

	FTimerHandle TimerHandle_RespawnDelay;
	FTimerDelegate Delegate;
	Delegate.BindUFunction(this, "RespawnPlayerElapsed", Player->GetController());

	float RespawnDelay = 2.0f;
	GetWorldTimerManager().SetTimer(TimerHandle_RespawnDelay, Delegate, RespawnDelay, false);
}

void ASGameModeBase::RespawnPlayerElapsed(AController* Controller)
{
	if (ensure(Controller))
	{
		Controller->UnPossess();
		
		RestartPlayer(Controller);
	}
	
}
