// Fill out your copyright notice in the Description page of Project Settings.

#include "SActionComponent.h"
#include "Engine/ActorChannel.h"
#include "LearnUnreal/LearnUnreal.h"
#include "Net/UnrealNetwork.h"
#include "SAction.h"

USActionComponent::USActionComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
    SetIsReplicatedByDefault(true);
    bReplicateUsingRegisteredSubObjectList = true;
}

void USActionComponent::AddAction(TSubclassOf<USAction> ActionClass, AActor* InstigatorActor)
{
    if (!ensure(ActionClass))
    {
        return;
    }
    USAction* NewAction = NewObject<USAction>(this, ActionClass);
    if (ensure(NewAction))
    {
        Actions.Add(NewAction);
        AddReplicatedSubObject(NewAction);

        if (NewAction->bAutoStart && NewAction->CanStart(InstigatorActor))
        {
            NewAction->StartAction(InstigatorActor);
        }
    }
}

void USActionComponent::RemoveAction(USAction* Action)
{
    if (ensure(Action) && !Action->IsRunning())
    {
        RemoveReplicatedSubObject(Action);
        Actions.Remove(Action);
    }
}

bool USActionComponent::StartActionByName(AActor* InstigatorActor, FName ActionName)
{
    for (USAction* Action : Actions)
    {
        if (Action && Action->ActionName == ActionName)
        {
            if (!Action->CanStart(InstigatorActor))
            {
                FString DebugMsg = FString::Printf(TEXT("Failed to start %f"), *ActionName.ToString());
                GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, DebugMsg);
                continue;
            }

            if (!GetOwner()->HasAuthority())
            {
                ServerStartAction(InstigatorActor, ActionName);
            }

            Action->StartAction(InstigatorActor);
            return true;
        }
    }
    return false;
}

bool USActionComponent::StopActionByName(AActor* InstigatorActor, FName ActionName)
{
    for (USAction* Action : Actions)
    {
        if (Action && Action->ActionName == ActionName)
        {
            if (Action->IsRunning())
            {
                Action->StopAction(InstigatorActor);
                return true;
            }
        }
    }
    return false;
}

void USActionComponent::BeginPlay()
{
    Super::BeginPlay();

    if (GetOwner()->HasAuthority())
    {
        for (TSubclassOf<USAction> Action : DefaultActions)
        {
            AddAction(Action, GetOwner());
        }
    }
}

void USActionComponent::ServerStartAction_Implementation(AActor* Instigator, FName ActionName)
{
    StartActionByName(Instigator, ActionName);
}

void USActionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    /*  FString DebugMsg = GetNameSafe(GetOwner()) + " : " + ActiveGameplayTags.ToStringSimple();
      GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::White, DebugMsg);*/

    for (USAction* Action : Actions)
    {
        FColor TextColor = Action->IsRunning() ? FColor::Blue : FColor::White;
        FString Msg = FString::Printf(TEXT("[%s] Action: %s, IsRunning: %s, Outer: %s"),
            *GetNameSafe(GetOwner()), *Action->ActionName.ToString(),
            Action->IsRunning() ? TEXT("true") : TEXT("false"), *GetNameSafe(Action->GetOuter()));
        LogOnScreen(this, Msg, TextColor, 0.f);
    }
}

void USActionComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(USActionComponent, Actions);
}
