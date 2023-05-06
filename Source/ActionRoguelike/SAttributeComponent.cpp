#include "SAttributeComponent.h"

USAttributeComponent::USAttributeComponent()
{
	Health = MaxHealth;
}

bool USAttributeComponent::ApplyHealthChange(float Delta)
{
	// Health += Delta;
	float PreviousHealth = Health;
	Health = FMath::Clamp(Health + Delta, 0.0f, MaxHealth);

	if (PreviousHealth == Health)
		return false;

	OnHealthChanged.Broadcast(nullptr, this, Health, Delta);
	
	return true;
}

bool USAttributeComponent::IsAlive() const
{
	return Health > 0.0f;
}
