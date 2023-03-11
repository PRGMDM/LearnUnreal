// Fill out your copyright notice in the Description page of Project Settings.

#include "SActionEffect.h"
#include "SActionComponent.h"

USActionEffect::USActionEffect()
{
    bAutoStart = true;
}

void USActionEffect::StartAction_Implementation(AActor* Instigator)
{
    Super::StartAction_Implementation(Instigator);

    if (Duration > 0.f)
    {
        FTimerDelegate Delegate;
        Delegate.BindUFunction(this, "StopAction", Instigator);
        GetWorld()->GetTimerManager().SetTimer(DurationHandle, Delegate, Duration, false);
    }

    if (Period > 0.f)
    {
        FTimerDelegate Delegate;
        Delegate.BindUFunction(this, "ApplyEffect", Instigator);
        GetWorld()->GetTimerManager().SetTimer(PeriodHandle, Delegate, Period, true);
    }
}

void USActionEffect::StopAction_Implementation(AActor* Instigator)
{
    if (GetWorld()->GetTimerManager().GetTimerRemaining(PeriodHandle) < KINDA_SMALL_NUMBER)
    {
        ApplyEffect(Instigator);
    }

    Super::StopAction_Implementation(Instigator);

    GetWorld()->GetTimerManager().ClearTimer(DurationHandle);
    GetWorld()->GetTimerManager().ClearTimer(PeriodHandle);
    USActionComponent* ActionComp = GetOwningComponent();
    if (ActionComp)
    {
        ActionComp->RemoveAction(this);
    }
}

void USActionEffect::ApplyEffect_Implementation(AActor* Instigator)
{
}