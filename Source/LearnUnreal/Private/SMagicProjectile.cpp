// Fill out your copyright notice in the Description page of Project Settings.

#include "SMagicProjectile.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "SAttributeComponent.h"
#include "SGameplayFunctionLibrary.h"

void ASMagicProjectile::PostInitializeComponents()
{
    Super::PostInitializeComponents();

    // More consistent to bind here compared to Constructor which may fail to bind if Blueprint was created before adding this binding (or when using hotreload)
    // PostInitializeComponent is the preferred way of binding any events.
    SphereComp->OnComponentBeginOverlap.AddDynamic(this, &ASMagicProjectile::OnActorOverlap);
}

void ASMagicProjectile::OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    UE_LOG(LogTemp, Log, TEXT("Projectile overlaps with %s"), *(OtherComp->GetReadableName()));
    if (OtherActor && OtherActor != GetInstigator())
    {
        if (USGameplayFunctionLibrary::ApplyDirectionalDamage(GetInstigator(), OtherActor, -20, SweepResult))
        {
            UE_LOG(LogTemp, Log, TEXT("Projectile Exploding."));
            Explode();
        }
    }
}
