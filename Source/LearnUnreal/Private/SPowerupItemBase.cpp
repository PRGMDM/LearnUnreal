// Fill out your copyright notice in the Description page of Project Settings.

#include "SPowerupItemBase.h"
#include "Components/SphereComponent.h"

// Sets default values
ASPowerupItemBase::ASPowerupItemBase()
{
    SphereComp = CreateDefaultSubobject<USphereComponent>("SphereComp");
    SphereComp->SetCollisionProfileName("Powerup");
    RootComponent = SphereComp;

    SpawnCollisionHandlingMethod = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

    bReplicates = true;

    Cooldown = 10.f;
}

void ASPowerupItemBase::Interact_Implementation(APawn* InstigatorPawn)
{
}

FText ASPowerupItemBase::GetInteractText_Implementation(APawn* InstigatorPawn)
{
    return FText::GetEmpty();
}

void ASPowerupItemBase::ShowItem()
{
    SetItemState(true);
}

void ASPowerupItemBase::HideItemAndCooldown()
{
    SetItemState(false);
    GetWorldTimerManager().SetTimer(TimerHandle_CooldownTimer, this, &ASPowerupItemBase::ShowItem, Cooldown);
}

void ASPowerupItemBase::SetItemState(bool bIsAvailable)
{
    SetActorEnableCollision(bIsAvailable);
    RootComponent->SetVisibility(bIsAvailable, true);
}
