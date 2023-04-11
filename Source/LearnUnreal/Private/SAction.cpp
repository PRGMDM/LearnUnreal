// Fill out your copyright notice in the Description page of Project Settings.

#include "SAction.h"
#include "LearnUnreal/LearnUnreal.h"
#include "Net/UnrealNetwork.h"
#include "SActionComponent.h"

void USAction::StartAction_Implementation(AActor* InstigatorActor)
{
    UE_LOG(LogTemp, Log, TEXT("Running: %s"), *GetNameSafe(this));
    // LogOnScreen(this, FString::Printf(TEXT("Started: %s"), *ActionName.ToString()), FColor::Green);
    USActionComponent* ActionComp = GetOwningComponent();
    ActionComp->ActiveGameplayTags.AppendTags(GrantsTags);

    RepData.bIsRunning = true;
    RepData.Instigator = InstigatorActor;

    if (GetOwningComponent()->GetOwner()->HasAuthority())
    {
        TimeStarted = GetWorld()->TimeSeconds;
    }

    GetOwningComponent()->OnActionStarted.Broadcast(GetOwningComponent(), this);
}

void USAction::StopAction_Implementation(AActor* InstigatorActor)
{
    UE_LOG(LogTemp, Log, TEXT("Stopped: %s"), *GetNameSafe(this));
    // LogOnScreen(this, FString::Printf(TEXT("Stopped: %s"), *ActionName.ToString()), FColor::Green);
    USActionComponent* ActionComp = GetOwningComponent();
    ActionComp->ActiveGameplayTags.RemoveTags(GrantsTags);
    RepData.bIsRunning = false;
    RepData.Instigator = InstigatorActor;

    GetOwningComponent()->OnActionStopped.Broadcast(GetOwningComponent(), this);
}

bool USAction::CanStart_Implementation(AActor* InstigatorActor)
{
    if (RepData.bIsRunning)
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
    return RepData.bIsRunning;
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

void USAction::OnRep_RepData()
{
    if (RepData.bIsRunning)
    {
        StartAction(RepData.Instigator);
    }
    else
    {
        StopAction(RepData.Instigator);
    }
}

USActionComponent* USAction::GetOwningComponent() const
{
    return Cast<USActionComponent>(GetOuter());
}

void USAction::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(USAction, RepData);
    DOREPLIFETIME(USAction, TimeStarted);
}
