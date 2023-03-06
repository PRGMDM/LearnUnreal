// Fill out your copyright notice in the Description page of Project Settings.

#include "SAttributeComponent.h"

// Sets default values for this component's properties
USAttributeComponent::USAttributeComponent()
{
    HealthMax = 100;
    Health = HealthMax;
}

bool USAttributeComponent::ApplyHealthChange(AActor* InstigatorActor, float Delta)
{
    if (!GetOwner()->CanBeDamaged())
    {
        return false;
    }
    float OldHealth = Health;
    Health = FMath::Clamp(Health + Delta, 0.f, HealthMax);
    float ActualDelta = Health - OldHealth;
    OnHealthChanged.Broadcast(InstigatorActor, this, Health, ActualDelta);
    return ActualDelta != 0;
}

bool USAttributeComponent::IsAlive() const
{
    return Health > 0 ? true : false;
}

bool USAttributeComponent::IsFullHealth() const
{
    return Health == HealthMax;
}

float USAttributeComponent::GetHealthMax() const
{
    return HealthMax;
}

bool USAttributeComponent::IsLowHealth() const
{
    return Health <= 0.2f * HealthMax;
}

bool USAttributeComponent::Kill(AActor* InstigatorActor)
{
    return ApplyHealthChange(InstigatorActor, -HealthMax);
}

USAttributeComponent* USAttributeComponent::GetAttributes(AActor* Actor)
{
    if (Actor)
    {
        return Cast<USAttributeComponent>(Actor->GetComponentByClass(StaticClass()));
    }
    return nullptr;
}

bool USAttributeComponent::IsActorAlive(AActor* Actor)
{
    USAttributeComponent* AttrComp = GetAttributes(Actor);
    if (ensure(AttrComp))
    {
        return AttrComp->IsAlive();
    }
    UE_LOG(LogTemp, Error, TEXT("Actor has no AttributeComponent."));
    return false;
}
