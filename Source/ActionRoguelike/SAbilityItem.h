// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SInteractItemBase.h"
#include "SAbilityItem.generated.h"

/**
 * 
 */
class USAction;
UCLASS()
class ACTIONROGUELIKE_API ASAbilityItem : public ASInteractItemBase
{
public:
	virtual void Interact_Implementation(APawn* InstigatorPawn) override;
	
private:
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere)
	TSubclassOf<USAction> ActionToGrant;
	
};
