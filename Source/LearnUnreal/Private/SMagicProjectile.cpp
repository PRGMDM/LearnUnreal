// Fill out your copyright notice in the Description page of Project Settings.

#include "SMagicProjectile.h"
#include "Kismet/GameplayStatics.h"
#include "SAttributeComponent.h"
#include "SGameplayFunctionLibrary.h"

void ASMagicProjectile::OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    if (OtherActor)
    {
        /* UGameplayStatics::PlayWorldCameraShake(this, ShakeEffect, Hit.Location, 0, 1000);

         USAttributeComponent* AttributeComp = USAttributeComponent::GetAttributes(OtherActor);
         if (AttributeComp)
         {
             AttributeComp->ApplyHealthChange(GetInstigator(), -20.f);
         }*/

        USGameplayFunctionLibrary::ApplyDirectionalDamage(GetInstigator(), OtherActor, -20.f, Hit);
    }
    // TODO: Do I put this in else? Also, seems the time for collision overhaul is finally here.
    ASProjectileBase::OnActorHit(HitComponent, OtherActor, OtherComp, NormalImpulse, Hit);
}
