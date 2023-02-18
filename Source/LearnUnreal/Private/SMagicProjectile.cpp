// Fill out your copyright notice in the Description page of Project Settings.

#include "SMagicProjectile.h"
#include "SAttributeComponent.h"

void ASMagicProjectile::OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    if (OtherActor) {
        USAttributeComponent* AttributeComp = Cast<USAttributeComponent>(OtherActor->GetComponentByClass(USAttributeComponent::StaticClass()));
        if (AttributeComp) {
            AttributeComp->ApplyHealthChange(-20.f);
        }
    }
    // TODO: Do I put this in else?
    ASProjectileBase::OnActorHit(HitComponent, OtherActor, OtherComp, NormalImpulse, Hit);
}
