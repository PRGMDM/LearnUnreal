// Fill out your copyright notice in the Description page of Project Settings.

#include "SPlayerState.h"

bool ASPlayerState::ModifyCredit(float Amount)
{
    UE_LOG(LogTemp, Log, TEXT("%f is given"), Amount);
    if (Amount > 0.f || Credits + Amount >= 0.f)
    {
        Credits += Amount;
        return true;
    }
    return false;
}
