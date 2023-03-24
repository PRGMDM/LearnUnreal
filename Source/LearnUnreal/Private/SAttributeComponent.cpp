// Fill out your copyright notice in the Description page of Project Settings.

#include "SAttributeComponent.h"
#include "SGameModeBase.h"

static TAutoConsoleVariable<float> CVarDamageMultiplier(TEXT("DamageMult"), 1.f, TEXT("Damager modifier for attribute component"), ECVF_Cheat);

// Sets default values for this component's properties
USAttributeComponent::USAttributeComponent()
{
    Health = HealthMax;
}

bool USAttributeComponent::ApplyHealthChange(AActor* InstigatorActor, float Delta)
{
    if (!GetOwner()->CanBeDamaged() && Delta < 0.f)
    {
        return false;
    }

    if (Delta < 0.f)
    {
        float Mult = CVarDamageMultiplier.GetValueOnGameThread();
        Delta *= Mult;
    }

    float OldHealth = Health;
    Health = FMath::Clamp(Health + Delta, 0.f, HealthMax);
    float ActualDelta = Health - OldHealth;
    OnHealthChanged.Broadcast(InstigatorActor, this, Health, ActualDelta);

    if (ActualDelta < 0.f)
    {
        ApplyRageChange(-ActualDelta * DamageToRageMultiplier);
    }

    if (ActualDelta < 0.f && Health == 0.f)
    {
        ASGameModeBase* GM = GetWorld()->GetAuthGameMode<ASGameModeBase>();
        if (GM)
        {
            GM->OnActorKilled(GetOwner(), InstigatorActor);
        }
    }

    return ActualDelta != 0;
}

bool USAttributeComponent::ApplyRageChange(float Delta)
{
    if (Delta < 0.f && (Rage + Delta < 0.f))
    {
        return false;
    }

    float OldRage = Rage;
    Rage = FMath::Clamp(Rage + Delta, 0.f, RageMax);
    float ActualDelta = Rage - OldRage;
    OnRageChanged.Broadcast(this, Rage);

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

float USAttributeComponent::GetRageMax() const
{
    return RageMax;
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
