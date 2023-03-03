// Fill out your copyright notice in the Description page of Project Settings.

#include "SMagicProjectile.h"
#include "Kismet/GameplayStatics.h"
#include "SAttributeComponent.h"

void ASMagicProjectile::OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    if (OtherActor)
    {
        UGameplayStatics::PlayWorldCameraShake(this, ShakeEffect, Hit.Location, 0, 1000);
        USAttributeComponent* AttributeComp = Cast<USAttributeComponent>(OtherActor->GetComponentByClass(USAttributeComponent::StaticClass()));
        if (AttributeComp)
        {
            AttributeComp->ApplyHealthChange(GetInstigator(), -20.f);
        }
    }
    // TODO: Do I put this in else?
    ASProjectileBase::OnActorHit(HitComponent, OtherActor, OtherComp, NormalImpulse, Hit);
}
