// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SGameplayInterface.h"
#include "SItemChest.generated.h"

UStaticMeshComponent;

UCLASS()
class LEARNUNREAL_API ASItemChest : public AActor, public ISGameplayInterface
{
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    ASItemChest();

    UPROPERTY(EditAnywhere)
    float TargetPitch = 110.f;

    void Interact_Implementation(APawn* InstigatorPawn);

protected:
    UPROPERTY(ReplicatedUsing = "OnRep_LidOpened")
    bool bLidOpened = false;

    UFUNCTION()
    void OnRep_LidOpened();

    UPROPERTY(VisibleAnywhere)
    UStaticMeshComponent* BaseMesh;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    UStaticMeshComponent* LidMesh;
};
