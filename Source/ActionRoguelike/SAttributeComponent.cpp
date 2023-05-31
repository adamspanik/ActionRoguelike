#include "SAttributeComponent.h"

#include "SGameModeBase.h"
#include "Net/UnrealNetwork.h"

static TAutoConsoleVariable<float> CVarDamageMultiplier(TEXT("su.DamageMultiplier"), 1.0f, TEXT("Global damage multipler for Attribute component"), ECVF_Cheat);

USAttributeComponent::USAttributeComponent()
{
	Health = MaxHealth;

	SetIsReplicatedByDefault(true);
}

bool USAttributeComponent::ApplyHealthChange(AActor* InstigatorActor, float Delta)
{
	if (!GetOwner()->CanBeDamaged() && Delta < 0.0f)
		return false;

	if (Delta < 0.0f)
	{
		Delta *= CVarDamageMultiplier.GetValueOnGameThread();
	}

	if (Delta < 0.0f)
	{
		const int RageAmount = -Delta;
		ApplyRageChange(InstigatorActor, RageAmount);
	}
	
	float OldHealth = Health;
	Health = FMath::Clamp(Health + Delta, 0.0f, MaxHealth);

	if (OldHealth == Health)
		return false;

	float ActualDelta = Health - OldHealth;
	//OnHealthChanged.Broadcast(InstigatorActor, this, Health, ActualDelta);
	if (ActualDelta != 0.0f)
	{
		MulticastHealthChanged(InstigatorActor, Health, ActualDelta);
	}

	if (ActualDelta < 0.0f && Health == 0.0f)
	{
		ASGameModeBase* GameMode = GetWorld()->GetAuthGameMode<ASGameModeBase>();
		if (GameMode)
		{
			GameMode->OnActorKilled(GetOwner(), InstigatorActor);
		}
	}
	
	return true;
}

bool USAttributeComponent::ApplyRageChange(AActor* InstigatorActor, float Delta)
{
	const float OldRage = Rage;
	Rage = FMath::Clamp(Rage + Delta, 0.0f, MaxRage);

	if (OldRage == Rage)
		return false;

	const float ActualDelta = Rage - OldRage;
	if (ActualDelta != 0.0f)
	{
		MulticastRageChanged(InstigatorActor, Rage, ActualDelta);
	}
	
	return true;
}

bool USAttributeComponent::IsAlive() const
{
	return Health > 0.0f;
}

USAttributeComponent* USAttributeComponent::GetAttributes(AActor* FromActor)
{
	if (!FromActor)
		return nullptr;

	return Cast<USAttributeComponent>(FromActor->GetComponentByClass(USAttributeComponent::StaticClass()));;
}

bool USAttributeComponent::IsActorAlive(AActor* Actor)
{
	USAttributeComponent* AttributeComponent = GetAttributes(Actor);
	if (!AttributeComponent)
		return false;
	
	return AttributeComponent->IsAlive();
}

bool USAttributeComponent::Kill(AActor* InstigatorActor)
{
	return ApplyHealthChange(InstigatorActor, -GetMaxHealth());
}

void USAttributeComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(USAttributeComponent, Health);
	DOREPLIFETIME(USAttributeComponent, MaxHealth);

	// DOREPLIFETIME_CONDITION(USAttributeComponent, MaxHealth, COND_OwnerOnly);
}

void USAttributeComponent::MulticastHealthChanged_Implementation(AActor* Instigator, float NewHealth, float Delta)
{
	OnHealthChanged.Broadcast(Instigator, this, NewHealth, Delta);
}

void USAttributeComponent::MulticastRageChanged_Implementation(AActor* Instigator, float NewRage, float Delta)
{
	OnRageChanged.Broadcast(Instigator, this, NewRage, Delta);
}

