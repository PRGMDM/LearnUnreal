// Fill out your copyright notice in the Description page of Project Settings.

#include "SAttributeComponent.h"

// Sets default values for this component's properties
USAttributeComponent::USAttributeComponent()
{
    HealthMax = 100;
    Health = HealthMax;
}

bool USAttributeComponent::ApplyHealthChange(float Delta)
{
    Health += Delta;
    Health = Health > HealthMax ? HealthMax : Health;
    OnHealthChanged.Broadcast(nullptr, this, Health, Delta);
    return true;
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
