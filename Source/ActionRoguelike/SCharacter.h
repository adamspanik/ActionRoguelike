// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SCharacter.generated.h"

class USAttributeComponent;
class USInteractionComponent;
class UCameraComponent;
class USpringArmComponent;

UCLASS()
class ACTIONROGUELIKE_API ASCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASCharacter();

protected:

	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* CameraComp;

	UPROPERTY(EditAnywhere, Category="Attack")
	UParticleSystem* ParticleSystem;

	UPROPERTY(VisibleAnywhere)
	USInteractionComponent* InteractionComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	USAttributeComponent* AttributeComp;

	UPROPERTY(EditAnywhere, Category="Attack")
	TSubclassOf<AActor> MagicProjectileClass;

	UPROPERTY(EditAnywhere, Category="Attack")
	TSubclassOf<AActor> DashProjectileClass;

	UPROPERTY(EditAnywhere, Category="Attack")
	TSubclassOf<AActor> BlackHoleProjectileClass;

	UPROPERTY(EditAnywhere, Category="Attack")
	UAnimMontage* AttackAnimation;
	
	FTimerHandle TimerHandle_PrimaryAttack;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void MoveForward(float Value);

	void MoveRight(float Value);
	
	void PrimaryAttack_TimeElapsed(TSubclassOf<AActor> ProjectileClass);
	
	void PrimaryAttack();

	void Dash();

	void SpecialAttack();

	void PrimaryInteract();

	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta);

	virtual void PostInitializeComponents() override;

public:
	USAttributeComponent* GetAttributeComponent() const { return AttributeComp; }
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
