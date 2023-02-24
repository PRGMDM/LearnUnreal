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

protected:
    UPROPERTY(VisibleAnywhere, Category = "Component")
    UStaticMeshComponent* MeshComp;

    void Interact_Implementation(APawn* InstigatorPawn) override;
};
