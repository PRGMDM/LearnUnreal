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
    ASItemChest();

    UPROPERTY(EditAnywhere)
    float TargetPitch = 110.f;

    void Interact_Implementation(APawn* InstigatorPawn);

    void OnActorLoaded_Implementation();

protected:
    UPROPERTY(ReplicatedUsing = "OnRep_LidOpened", BlueprintReadonly, SaveGame)
    bool bLidOpened = false;

    UFUNCTION()
    void OnRep_LidOpened();

    UPROPERTY(VisibleAnywhere)
    UStaticMeshComponent* BaseMesh;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    UStaticMeshComponent* LidMesh;
};
