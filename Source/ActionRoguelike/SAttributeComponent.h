// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SAttributeComponent.generated.h"

class USAttributeComponent;
// DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnHealthChanged, AActor*, Instigator, USAttributeComponent*, OwningComp, float, NewHealth, float, Delta);
// DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnRageChanged, AActor*, Instigator, USAttributeComponent*, OwningComp, float, NewRage, float, Delta);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnAttributeChanged, AActor*, Instigator, USAttributeComponent*, OwningComp, float, NewValue, float, Delta);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ACTIONROGUELIKE_API USAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	USAttributeComponent();

	UFUNCTION(BlueprintCallable, Category="Attributes")
	static USAttributeComponent* GetAttributes(AActor* FromActor);

	UFUNCTION(BlueprintCallable, Category="Attributes", meta=(DisplayName = "IsAlive"))
	static bool IsActorAlive(AActor* Actor);

protected:
	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadOnly, Category="Attributes")
	float Health;

	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadOnly, Category="Attributes")
	float MaxHealth = 200.0f;

	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadOnly, Category="Attributes")
	float Rage;

	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadOnly, Category="Attributes")
	float MaxRage = 100.0f;

	UFUNCTION(NetMulticast, Reliable)
	void MulticastHealthChanged(AActor* Instigator, float NewHealth, float Delta);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRageChanged(AActor* Instigator, float NewRage, float Delta);

public:
	UPROPERTY(BlueprintAssignable)
	FOnAttributeChanged OnHealthChanged;

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChanged OnRageChanged;
	
	UFUNCTION(BlueprintCallable, Category="Attributes")
	bool ApplyHealthChange(AActor* InstigatorActor, float Delta);

	UFUNCTION(BlueprintCallable, Category="Attributes")
	bool ApplyRageChange(AActor* InstigatorActor, float Delta);

	UFUNCTION(BlueprintCallable)
	bool IsAlive() const;

	UFUNCTION(BlueprintCallable)
	float GetHealth() { return Health; }

	UFUNCTION(BlueprintCallable)
	float GetMaxHealth() { return MaxHealth; }

	UFUNCTION(BlueprintCallable)
	float GetRage() { return Rage; }

	UFUNCTION(BlueprintCallable)
	float GetMaxRage() { return MaxRage; }

	UFUNCTION(BlueprintCallable)
	bool Kill(AActor* InstigatorActor);
	
};
