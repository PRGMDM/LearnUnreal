// Fill out your copyright notice in the Description page of Project Settings.

#include "SPlayerState.h"

bool ASPlayerState::ModifyCredit(float Amount)
{
    UE_LOG(LogTemp, Log, TEXT("%f credits are given to player"), Amount);
    if (Amount > 0.f || Credits + Amount >= 0.f)
    {
        Credits += Amount;
        OnCreditsChanged.Broadcast();
        return true;
    }
    return false;
}

float ASPlayerState::GetCredits() const
{
    return Credits;
}
