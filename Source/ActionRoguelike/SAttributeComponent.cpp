#include "SAttributeComponent.h"

USAttributeComponent::USAttributeComponent()
{
	Health = MaxHealth;
}

bool USAttributeComponent::ApplyHealthChange(float Delta)
{
	float OldHealth = Health;
	Health = FMath::Clamp(Health + Delta, 0.0f, MaxHealth);

	float ActualDelta = Health - OldHealth;

	if (OldHealth == Health)
		return false;
	
	OnHealthChanged.Broadcast(nullptr, this, Health, ActualDelta);
	
	return true;
}

bool USAttributeComponent::IsAlive() const
{
	return Health > 0.0f;
}
