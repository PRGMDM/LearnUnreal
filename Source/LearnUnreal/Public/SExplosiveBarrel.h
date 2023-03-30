// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SExplosiveBarrel.generated.h"

class UStaticMeshComponent;
class URadialForceComponent;

UCLASS()
class LEARNUNREAL_API ASExplosiveBarrel : public AActor
{
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    ASExplosiveBarrel();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    UPROPERTY(VisibleAnywhere)
    UStaticMeshComponent* MeshComp;

    UPROPERTY(VisibleAnywhere)
    URadialForceComponent* ForceComp;

    UFUNCTION()
    void OnActorHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
};

// TODO: How to make this work in multiplayer:
// 1. Make sure there is a server RPC that triggers the explosion.
// 2. Make the impulse firing multicast.
