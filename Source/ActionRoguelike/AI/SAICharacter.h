// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SAICharacter.generated.h"

class USWorldUserWidget;
class USAttributeComponent;
class UPawnSensingComponent;

UCLASS()
class ACTIONROGUELIKE_API ASAICharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASAICharacter();

protected:
	void SetTargetActor(AActor* Targer);
	
	virtual void PostInitializeComponents() override;

	UPROPERTY(VisibleAnywhere, Category="UI")
	USWorldUserWidget* ActiveHealthBar;
	
	UPROPERTY(EditAnywhere, Category="UI")
	TSubclassOf<UUserWidget> HealthBarWidgetClass;
	
	UPROPERTY(VisibleAnywhere, Category="Components")
	UPawnSensingComponent* PawnSensingComponent;

	UFUNCTION()
	void OnPawnSeen(APawn* Pawn);

	UPROPERTY(VisibleAnywhere, Category="Components")
	USAttributeComponent* AttributeComponent;

	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta);

};
