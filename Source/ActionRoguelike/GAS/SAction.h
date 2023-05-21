﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/Object.h"
#include "SAction.generated.h"

class USActionComponent;

UCLASS(Blueprintable)
class ACTIONROGUELIKE_API USAction : public UObject
{
	GENERATED_BODY()

public:
	/* Start immediately when added to an action component */
	UPROPERTY(EditDefaultsOnly, Category="Action")
	bool bAutoStart;
	
	UFUNCTION(BlueprintNativeEvent, Category="Action")
	bool CanStartAction(AActor* Instigator);

	UFUNCTION(BlueprintNativeEvent, Category="Action")
	void StartAction(AActor* Instigator);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Action")
	void StopAction(AActor* Instigator);

	UPROPERTY(EditDefaultsOnly, Category="Action")
	FName ActionName;

	virtual UWorld* GetWorld() const override;

	UFUNCTION(BlueprintCallable, Category="Action")
	bool IsRunning() const { return bIsRunning; }

protected:
	UFUNCTION(BlueprintCallable, Category="Action")
	USActionComponent* GetOwningComponent() const;
	
	UPROPERTY(EditDefaultsOnly, Category="Tags")
	FGameplayTagContainer GrantsTags;

	UPROPERTY(EditDefaultsOnly, Category="Tags")
	FGameplayTagContainer BlockedTags;

	bool bIsRunning;	
};
