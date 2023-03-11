// Fill out your copyriht notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayTagContainer.h"
#include "SProjectileBase.h"
#include "SMagicProjectile.generated.h"

class USActionEffect;
// TODO: Maybe add a lifespan, destroy self if go too far/after some time.

UCLASS()
class LEARNUNREAL_API ASMagicProjectile : public ASProjectileBase
{
    GENERATED_BODY()

protected:
    UPROPERTY(EditAnywhere, Category = "Effect")
    TSubclassOf<UCameraShakeBase> ShakeEffect;

    UPROPERTY(EditDefaultsOnly, Category = "Damage")
    FGameplayTag ParryTag;

    UPROPERTY(EditDefaultsOnly, Category = "Damage")
    float DamageAmount;

    UPROPERTY(EditDefaultsOnly, Category = "Damage")
    TSubclassOf<USActionEffect> BurningEffectClass;

    UFUNCTION()
    void OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    virtual void PostInitializeComponents() override;
};
