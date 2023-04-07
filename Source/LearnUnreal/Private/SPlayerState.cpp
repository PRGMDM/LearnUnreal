// Fill out your copyright notice in the Description page of Project Settings.

#include "SPlayerState.h"
#include "SSaveGame.h"

bool ASPlayerState::ModifyCredit(int32 Amount)
{
    UE_LOG(LogTemp, Log, TEXT("%f credits are given to player"), Amount);
    if (Amount > 0 || Credits + Amount >= 0)
    {
        Credits += Amount;
        OnCreditsChanged.Broadcast();
        return true;
    }
    return false;
}

int32 ASPlayerState::GetCredits() const
{
    return Credits;
}

void ASPlayerState::SavePlayerState_Implementation(USSaveGame* SaveGame)
{
    if (SaveGame)
    {
        SaveGame->Credits = Credits;
    }
}

void ASPlayerState::LoadPlayerState_Implementation(USSaveGame* SaveGame)
{
    if (SaveGame)
    {
        Credits = SaveGame->Credits;
    }
}
