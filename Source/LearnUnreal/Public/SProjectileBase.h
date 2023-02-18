// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SProjectileBase.generated.h"

class USphereComponent;
class UProjectileMovementComponent;
class UParticleSystemComponent;
class UAudioComponent;

UCLASS(ABSTRACT)
class LEARNUNREAL_API ASProjectileBase : public AActor {
    GENERATED_BODY()

public:
    ASProjectileBase();

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category = "Components")
    USphereComponent* SphereComp;

    UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category = "Components")
    UProjectileMovementComponent* MoveComp;

    UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category = "Components")
    UParticleSystemComponent* EffectComp;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
    UAudioComponent* AudioComp;

    UPROPERTY(EditDefaultsOnly, Category = "Effects")
    UParticleSystem* ImpactVFX;

    UPROPERTY(EditDefaultsOnly, Category = "Effects")
    USoundBase* ImpactSound;

    UFUNCTION()
    virtual void OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, FVector NormalImpulse,
        const FHitResult& Hit);

    UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
    void Explode();

    virtual void PostInitializeComponents() override;
};
