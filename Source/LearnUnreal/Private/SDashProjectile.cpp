// Fill out your copyright notice in the Description page of Project Settings.

#include "SDashProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

ASDashProjectile::ASDashProjectile()
{
    TeleportDelay = 0.2f;
    DetonateDelay = 0.2f;

    MoveComp->InitialSpeed = 6000.f;
}

void ASDashProjectile::BeginPlay()
{
    Super::BeginPlay();
    GetWorldTimerManager().SetTimer(TimerHandle_DelayedDetonate, this, &ASDashProjectile::Explode, DetonateDelay);
}

void ASDashProjectile::Explode_Implementation()
{
    GetWorldTimerManager().ClearTimer(TimerHandle_DelayedDetonate);
    UGameplayStatics::SpawnEmitterAtLocation(this, ImpactVFX, GetActorLocation(), GetActorRotation());

    EffectComp->DeactivateSystem();

    MoveComp->StopMovementImmediately();
    SetActorEnableCollision(false);

    FTimerHandle TimerHandle_DelayedTeleport;
    GetWorldTimerManager().SetTimer(TimerHandle_DelayedTeleport, this, &ASDashProjectile::TeleportInstigator, TeleportDelay);
}

void ASDashProjectile::TeleportInstigator()
{
    AActor* MyInstigator = GetInstigator();
    if (ensure(MyInstigator))
    {
        bool result = MyInstigator->TeleportTo(GetActorLocation(), MyInstigator->GetActorRotation(), false, true);
        UE_LOG(LogTemp, Warning, TEXT("Teleporting to %f %f %f, %s"), GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z, result ? TEXT("True") : TEXT("False"));
    }
}
