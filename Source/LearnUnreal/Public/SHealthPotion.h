// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SPowerupItemBase.h"
#include "SHealthPotion.generated.h"

class UStaticMeshComponent;

UCLASS()
class LEARNUNREAL_API ASHealthPotion : public ASPowerupItemBase
{
    GENERATED_BODY()

    ASHealthPotion();

    void Interact_Implementation(APawn* InstigatorPawn) override;

    FText GetInteractText_Implementation(APawn* InstigatorPawn) override;

protected:
    UPROPERTY(VisibleAnywhere, Category = "Component")
    UStaticMeshComponent* MeshComp;

    UPROPERTY(EditDefaultsOnly, Category = "Credits")
    float Cost = 30;
};
