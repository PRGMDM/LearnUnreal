// Fill out your copyright notice in the Description page of Project Settings.

#include "SMagicProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "SActionComponent.h"
#include "SActionEffect.h"
#include "SAttributeComponent.h"
#include "SCharacter.h"
#include "SGameplayFunctionLibrary.h"

ASMagicProjectile::ASMagicProjectile()
{
    InitialLifeSpan = 10.f;
}

void ASMagicProjectile::PostInitializeComponents()
{
    Super::PostInitializeComponents();
    SphereComp->OnComponentBeginOverlap.AddDynamic(this, &ASMagicProjectile::OnActorOverlap);
}

void ASMagicProjectile::OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    UE_LOG(LogTemp, Log, TEXT("Projectile overlaps with %s"), *GetNameSafe(OtherActor));
    if (OtherActor && OtherActor != GetInstigator())
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

        if (USGameplayFunctionLibrary::ApplyDirectionalDamage(GetInstigator(), OtherActor, -DamageAmount, SweepResult))
        {
            Explode();
            if (ActionComp && HasAuthority())
            {
                ActionComp->AddAction(BurningEffectClass, GetInstigator());
            }
        }
    }
}
