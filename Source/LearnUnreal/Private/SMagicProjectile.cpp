// Fill out your copyright notice in the Description page of Project Settings.

#include "SMagicProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "SActionComponent.h"
#include "SAttributeComponent.h"
#include "SCharacter.h"
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
    if (OtherActor && OtherActor != GetInstigator()) // TODO: Can probably remove the ignore actor from projectile spawning.
    {
        // To access GameTags in c++:
        // Static FGameplayTag Tag = FGameplayTag::RequestGameplayTag("Tag.Name");
        // Static so that only run the request logic once

        USActionComponent* ActionComp = Cast<USActionComponent>(OtherActor->GetComponentByClass(USActionComponent::StaticClass()));
        if (ActionComp && ActionComp->ActiveGameplayTags.HasTag(ParryTag))
        {
            MoveComp->Velocity = -MoveComp->Velocity;
            SetInstigator(Cast<APawn>(OtherActor));
            return;
        }

        if (USGameplayFunctionLibrary::ApplyDirectionalDamage(GetInstigator(), OtherActor, -20, SweepResult))
        {
            UE_LOG(LogTemp, Log, TEXT("Projectile Exploding."));
            Explode();
        }
    }
}
