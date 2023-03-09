// Fill out your copyright notice in the Description page of Project Settings.

#include "SAction.h"
#include "SActionComponent.h"

void USAction::StartAction_Implementation(AActor* InstigatorActor)
{
    UE_LOG(LogTemp, Log, TEXT("Running: %s"), *GetNameSafe(this));
    USActionComponent* ActionComp = GetOwningComponent();
    ActionComp->ActiveGameplayTags.AppendTags(GrantsTags);
    bIsRunning = true;
}

void USAction::StopAction_Implementation(AActor* InstigatorActor)
{
    UE_LOG(LogTemp, Log, TEXT("Stopped: %s"), *GetNameSafe(this));
    ensureAlways(bIsRunning);
    USActionComponent* ActionComp = GetOwningComponent();
    ActionComp->ActiveGameplayTags.RemoveTags(GrantsTags);
    bIsRunning = false;
}

bool USAction::CanStart_Implementation(AActor* Instigator)
{
    if (bIsRunning)
    {
        return false;
    }

    USActionComponent* ActionComp = GetOwningComponent();
    if (ActionComp->ActiveGameplayTags.HasAny(BlockedTags))
    {
        return false;
    }
    return true;
}

bool USAction::IsRunning() const
{
    return bIsRunning;
}

UWorld* USAction::GetWorld() const
{
    UActorComponent* Comp = Cast<UActorComponent>(GetOuter());
    if (Comp)
    {
        return Comp->GetWorld();
    }
    return nullptr;
}

USActionComponent* USAction::GetOwningComponent() const
{
    return Cast<USActionComponent>(GetOuter());
}
