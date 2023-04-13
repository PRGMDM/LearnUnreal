// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SGameplayInterface.h"
#include "SPowerupItemBase.generated.h"

class USphereComponent;

UCLASS()
class LEARNUNREAL_API ASPowerupItemBase : public AActor, public ISGameplayInterface
{
    GENERATED_BODY()

public:
    ASPowerupItemBase();

    void Interact_Implementation(APawn* InstigatorPawn) override;

    FText GetInteractText_Implementation(APawn* InstigatorPawn) override;

protected:
    UPROPERTY(VisibleAnywhere, Category = "Component")
    USphereComponent* SphereComp;

    UPROPERTY(EditAnywhere, Category = "Powerup")
    float Cooldown;

    FTimerHandle TimerHandle_CooldownTimer;

    UFUNCTION()
    void ShowItem();

    void HideItemAndCooldown();

    void SetItemState(bool bIsAvailable);
};
