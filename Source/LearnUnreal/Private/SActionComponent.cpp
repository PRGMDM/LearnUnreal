// Fill out your copyright notice in the Description page of Project Settings.

#include "SActionComponent.h"
#include "SAction.h"

USActionComponent::USActionComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
}

void USActionComponent::AddAction(TSubclassOf<USAction> ActionClass)
{
    if (!ensure(ActionClass))
    {
        return;
    }
    USAction* NewAction = NewObject<USAction>(this, ActionClass);
    if (ensure(NewAction))
    {
        Actions.Add(NewAction);
    }
}

bool USActionComponent::StartActionByName(AActor* InstigatorActor, FName ActionName)
{
    for (USAction* Action : Actions)
    {
        if (Action && Action->ActionName == ActionName)
        {
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
            Action->StopAction(InstigatorActor);
            return true;
        }
    }
    return false;
}

void USActionComponent::BeginPlay()
{
    Super::BeginPlay();
    for (TSubclassOf<USAction> Action : DefaultActions)
    {
        AddAction(Action);
    }
}

void USActionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}
