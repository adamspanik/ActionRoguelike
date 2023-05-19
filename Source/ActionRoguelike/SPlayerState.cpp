// Fill out your copyright notice in the Description page of Project Settings.


#include "SPlayerState.h"

int32 ASPlayerState::GetCredits() const
{
	return  Credits;
}

bool ASPlayerState::AddCredits(int32 Delta)
{
	if (!ensure(Delta > 0.0f))
		return false;
	
	Credits += Delta;
	
	OnCreditsChanged.Broadcast(this, Credits, Delta);
	
	return true;
}

bool ASPlayerState::RemoveCredits(int32 Delta)
{
	if (!ensure(Delta > 0.0f))
		return false;

	if (Credits < Delta)
		return false;
	
	Credits -= Delta;

	OnCreditsChanged.Broadcast(this, Credits, Delta);
	
	return true;
}
