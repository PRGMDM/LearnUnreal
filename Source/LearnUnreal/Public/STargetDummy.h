// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "STargetDummy.generated.h"

class USAttributeComponent;

UCLASS()
class LEARNUNREAL_API ASTargetDummy : public AActor
{
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    ASTargetDummy();

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadonly)
    UStaticMeshComponent* MeshComp;

    UPROPERTY(VisibleAnywhere, BlueprintReadonly)
    USAttributeComponent* AttributeComp;

    UFUNCTION()
    void OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta);
};
