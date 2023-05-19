// Fill out your copyright notice in the Description page of Project Settings.


#include "SCreditItem.h"

#include "SPlayerState.h"

ASCreditItem::ASCreditItem()
{
}

void ASCreditItem::Interact_Implementation(APawn* InstigatorPawn)
{
	ASPlayerState* PlayerState = InstigatorPawn->GetPlayerState<ASPlayerState>();
	if (!PlayerState)
		return;

	PlayerState->AddCredits(Value);
	HideAndCooldownPowerUp();
}
