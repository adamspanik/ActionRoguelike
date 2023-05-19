// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SInteractItemBase.h"
#include "SCreditItem.generated.h"

/**
 * 
 */
UCLASS(ABSTRACT)
class ACTIONROGUELIKE_API ASCreditItem : public ASInteractItemBase
{
	GENERATED_BODY()

public:
	ASCreditItem();

private:
	virtual void Interact_Implementation(APawn* InstigatorPawn) override;

	UPROPERTY(EditAnywhere)
	int32 Value = 5;
};
