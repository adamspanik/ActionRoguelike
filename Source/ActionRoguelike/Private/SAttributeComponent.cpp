#include "SAttributeComponent.h"

USAttributeComponent::USAttributeComponent()
{
	Health = 200;
}

bool USAttributeComponent::ApplyHealthChange(float Delta)
{
	Health += Delta;

	OnHealthChanged.Broadcast(nullptr, this, Health, Delta);
	
	return true;
}
