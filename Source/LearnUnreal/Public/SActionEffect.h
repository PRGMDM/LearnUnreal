// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SAction.h"
#include "SActionEffect.generated.h"

UCLASS()
class LEARNUNREAL_API USActionEffect : public USAction
{
    GENERATED_BODY()

public:
    USActionEffect();

    void StartAction_Implementation(AActor* Instigator) override;

    void StopAction_Implementation(AActor* Instigator) override;

    UFUNCTION(BlueprintCallable, Category = "Action")
    float GetTimeRemaining() const;

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effect")
    float Duration = 0.f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effect")
    float Period = 0.f;

    FTimerHandle DurationHandle;
    FTimerHandle PeriodHandle;

    UFUNCTION(BlueprintNativeEvent, Category = "Effect")
    void ApplyEffect(AActor* Instigator);
};
