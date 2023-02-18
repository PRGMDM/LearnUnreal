// Fill out your copyriht notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SProjectileBase.h"
#include "SMagicProjectile.generated.h"

UCLASS()
class LEARNUNREAL_API ASMagicProjectile : public ASProjectileBase {
    GENERATED_BODY()

protected:
    virtual void OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, FVector NormalImpulse,
        const FHitResult& Hit) override;
};
// TODO: Maybe ditch the hit event and go for the overlap event and then decide what to do with the projectile.
// i.e. go through if the other actor is owner/teammate and explode/deal damage and destroy self in other cases.

// TODO: Maybe add a lifespan, destroy self if go too far/after some time.
