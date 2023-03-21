// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SPowerupItemBase.h"
#include "SCoin.generated.h"

/**
 *
 */
UCLASS()
class LEARNUNREAL_API ASCoin : public ASPowerupItemBase
{
    GENERATED_BODY()
public:
    ASCoin();

    void Interact_Implementation(APawn* InstigatorPawn) override;

protected:
    UPROPERTY(VisibleAnywhere, Category = "Component")
    TObjectPtr<UStaticMeshComponent> MeshComp;

    UPROPERTY(EditDefaultsOnly, Category = "Credits")
    float GrantCredits = 50;
};
