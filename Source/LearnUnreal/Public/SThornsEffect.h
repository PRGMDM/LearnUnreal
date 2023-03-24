// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SActionEffect.h"
#include "SThornsEffect.generated.h"

class USAttributeComponent;

// Effect that reflects damage to attacker.
UCLASS()
class LEARNUNREAL_API USThornsEffect : public USActionEffect
{
    GENERATED_BODY()

public:
    USThornsEffect();

    void StartAction_Implementation(AActor* Instigator) override;

    void StopAction_Implementation(AActor* Instigator) override;

protected:
    UPROPERTY(EditDefaultsOnly)
    float DamageMultiplier = 0.3f;

    UFUNCTION()
    void ReflectDamage(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta);
};
