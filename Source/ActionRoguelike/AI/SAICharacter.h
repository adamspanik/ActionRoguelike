// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActionRoguelike/GAS/SActionComponent.h"
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

	UFUNCTION(BlueprintCallable, Category = "AI")
	AActor* GetTargetActor() const;
	
	virtual void PostInitializeComponents() override;

	/* Key for AI Blackboard 'TargetActor' */
	UPROPERTY(VisibleAnywhere, Category = "Effects")
	FName TargetActorKey = "TargetActor";
	
	UPROPERTY(VisibleAnywhere, Category="UI")
	USWorldUserWidget* ActiveHealthBar;
	
	UPROPERTY(EditAnywhere, Category="UI")
	TSubclassOf<UUserWidget> HealthBarWidgetClass;

	/* Widget to display when bot first sees a player. */
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> SpottedWidgetClass;
	
	UPROPERTY(VisibleAnywhere, Category="Components")
	UPawnSensingComponent* PawnSensingComponent;

	UFUNCTION()
	void OnPawnSeen(APawn* Pawn);

	UPROPERTY(VisibleAnywhere, Category="Components")
	USAttributeComponent* AttributeComponent;

	UPROPERTY(VisibleAnywhere, Category="Components")
	USActionComponent* ActionComponent;

	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta);

};
