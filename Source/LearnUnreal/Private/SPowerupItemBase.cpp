// Fill out your copyright notice in the Description page of Project Settings.

#include "SPowerupItemBase.h"
#include "Components/SphereComponent.h"
#include "Net/UnrealNetwork.h"

// Sets default values
ASPowerupItemBase::ASPowerupItemBase()
{
    SphereComp = CreateDefaultSubobject<USphereComponent>("SphereComp");
    SphereComp->SetCollisionProfileName("Powerup");
    RootComponent = SphereComp;

    SpawnCollisionHandlingMethod = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

    bReplicates = true;
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
    bIsAvailable = true;
    OnRep_bIsAvailable();
}

void ASPowerupItemBase::HideItemAndCooldown()
{
    bIsAvailable = false;
    OnRep_bIsAvailable();
    GetWorldTimerManager().SetTimer(TimerHandle_CooldownTimer, this, &ASPowerupItemBase::ShowItem, Cooldown);
}

void ASPowerupItemBase::OnRep_bIsAvailable()
{
    UE_LOG(LogTemp, Log, TEXT("is avail rep'd: %s"), bIsAvailable ? TEXT("true") : TEXT("false"))
    SetActorEnableCollision(bIsAvailable);
    RootComponent->SetVisibility(bIsAvailable, true);
}

void ASPowerupItemBase::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(ASPowerupItemBase, bIsAvailable);
}
