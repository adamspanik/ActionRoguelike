// Fill out your copyright notice in the Description page of Project Settings.
#include "SGameModeBase.h"

#include "EngineUtils.h"
#include "SAttributeComponent.h"
#include "SCharacter.h"
#include "SPlayerState.h"
#include "AI/SAICharacter.h"
#include "EnvironmentQuery/EnvQueryManager.h"

static TAutoConsoleVariable<bool> CVarSpawnBots(TEXT("su.SpawnBots"), true, TEXT("Enable spawning bots via timer"), ECVF_Cheat);

ASGameModeBase::ASGameModeBase()
{
	PlayerStateClass = ASPlayerState::StaticClass();
}

void ASGameModeBase::StartPlay()
{
	Super::StartPlay();

	if (ensure(PowerUpClasses.Num() > 0))
	{
		UEnvQueryInstanceBlueprintWrapper* QueryInstanceBlueprintWrapper = UEnvQueryManager::RunEQSQuery(this, PowerUpSpawnQuery, this, EEnvQueryRunMode::AllMatching, nullptr);

		if (ensure(QueryInstanceBlueprintWrapper))
		{
			QueryInstanceBlueprintWrapper->GetOnQueryFinishedEvent().AddDynamic(this, &ThisClass::OnPowerUpSpawnQueryCompleted);
		}
	}

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
		QueryInstanceBlueprintWrapper->GetOnQueryFinishedEvent().AddDynamic(this, &ThisClass::OnBotSpawnQueryCompleted);
	}
}

void ASGameModeBase::OnBotSpawnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus)
{
	if (QueryStatus != EEnvQueryStatus::Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("SPAWN FAILED OnBotSpawnQueryCompleted"));
		return;
	}
	
	TArray<FVector> Locations = QueryInstance->GetResultsAsLocations();

	if (Locations.IsValidIndex(0))
	{
		GetWorld()->SpawnActor<AActor>(MinionClass, Locations[0], FRotator::ZeroRotator);

		DrawDebugSphere(GetWorld(), Locations[0], 50.0f, 20, FColor::Blue, false,60.0f);
	}
}

void ASGameModeBase::OnPowerUpSpawnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus)
{
	if (QueryStatus != EEnvQueryStatus::Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("SPAWN FAILED OnPowerUpSpawnQueryCompleted"));
		return;
	}
	
	TArray<FVector> Locations = QueryInstance->GetResultsAsLocations();
	
	// Keep used locations to easily check distance between points
	TArray<FVector> UsedLocations;
	
	int32 SpawnCounter = 0;
	// Break out if we reached the desired count or if we have no more potential positions remaining
	while (SpawnCounter < DesiredPowerUpCount && Locations.Num() > 0)
	{
		// Pick a random location from remaining points.
		int32 RandomLocationIndex = FMath::RandRange(0, Locations.Num() - 1);

		FVector PickedLocation = Locations[RandomLocationIndex];
		// Remove to avoid picking again
		Locations.RemoveAt(RandomLocationIndex);

		// Check minimum distance requirement
		bool bValidLocation = true;
		for (FVector OtherLocation : UsedLocations)
		{
			float DistanceTo = (PickedLocation - OtherLocation).Size();

			if (DistanceTo < RequiredPowerUpDistance)
			{
				// Show skipped locations due to distance
				//DrawDebugSphere(GetWorld(), PickedLocation, 50.0f, 20, FColor::Red, false, 10.0f);

				// too close, skip to next attempt
				bValidLocation = false;
				break;
			}
		}

		// Failed the distance test
		if (!bValidLocation)
		{
			continue;
		}

		// Pick a random powerup-class
		int32 RandomClassIndex = FMath::RandRange(0, PowerUpClasses.Num() - 1);
		TSubclassOf<AActor> RandomPowerupClass = PowerUpClasses[RandomClassIndex];

		GetWorld()->SpawnActor<AActor>(RandomPowerupClass, PickedLocation, FRotator::ZeroRotator);

		// Keep for distance checks
		UsedLocations.Add(PickedLocation);
		SpawnCounter++;
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

	// Give Credits for kill
	APawn* KillerPawn = Cast<APawn>(Killer);
	if (KillerPawn)
	{
		if (ASPlayerState* PS = KillerPawn->GetPlayerState<ASPlayerState>()) // < can cast and check for nullptr within if-statement.
			{
			PS->AddCredits(CreditsPerKill);
			}
	}
	
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
