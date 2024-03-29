// Fill out your copyright notice in the Description page of Project Settings.

#include "SAttributeComponent.h"
#include "Net/UnrealNetwork.h"
#include "SGameModeBase.h"

static TAutoConsoleVariable<float> CVarDamageMultiplier(TEXT("DamageMult"), 1.f, TEXT("Damager modifier for attribute component"), ECVF_Cheat);

// Sets default values for this component's properties
USAttributeComponent::USAttributeComponent()
{
    Health = HealthMax;
    SetIsReplicatedByDefault(true);
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
    float NewHealth = FMath::Clamp(Health + Delta, 0.f, HealthMax);
    float ActualDelta = NewHealth - OldHealth;
    if (GetOwner()->HasAuthority())
    {
        Health = NewHealth;
        if (ActualDelta != 0.f)
        {
            MulticastHealthChanged(InstigatorActor, Health, ActualDelta);
        }

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
    float NewRage = FMath::Clamp(Rage + Delta, 0.f, RageMax);
    float ActualDelta = NewRage - OldRage;
    if (GetOwner()->HasAuthority())
    {
        Rage = NewRage;
        MuticastRageChanged(NewRage);
    }
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

float USAttributeComponent::GetHealth() const
{
    return Health;
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

void USAttributeComponent::MuticastRageChanged_Implementation(float NewRage)
{
    OnRageChanged.Broadcast(this, NewRage);
}

void USAttributeComponent::MulticastHealthChanged_Implementation(AActor* InstigatorActor, float NewHealth, float Delta)
{
    OnHealthChanged.Broadcast(InstigatorActor, this, NewHealth, Delta);
}

void USAttributeComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(USAttributeComponent, Health);
    DOREPLIFETIME(USAttributeComponent, HealthMax);
    DOREPLIFETIME(USAttributeComponent, Rage);
    DOREPLIFETIME(USAttributeComponent, RageMax);
}
