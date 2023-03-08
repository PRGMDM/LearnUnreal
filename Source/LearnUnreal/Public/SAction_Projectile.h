// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SAction.h"
#include "SAction_Projectile.generated.h"

class UAnimMontage;
class UParticleSystem;
class ASCharacter;

UCLASS()
class LEARNUNREAL_API USAction_Projectile : public USAction
{
    GENERATED_BODY()

public:
    USAction_Projectile();

    virtual void StartAction_Implementation(AActor* InstigatorActor) override;

protected:
    UPROPERTY(EditAnywhere)
    TSubclassOf<AActor> ProjectileClass;

    UPROPERTY(EditAnywhere, Category = "Attack")
    UAnimMontage* AttackAnim;

    UPROPERTY(EditAnywhere, Category = "Effects")
    UParticleSystem* AttackVFX;

    UPROPERTY(VisibleAnywhere, Category = "Effects")
    FName MuzzleSocketName;

    UPROPERTY(VisibleAnywhere, Category = "Effects")
    float AttackAnimDelay;

    UFUNCTION()
    void Attack_Elapsed(ASCharacter* InstigatorActor);
};
