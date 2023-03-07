// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SAction.generated.h"

UCLASS(Blueprintable)
class LEARNUNREAL_API USAction : public UObject
{
    GENERATED_BODY()

public:
    UPROPERTY(EditDefaultsOnly, Category = "Action")
    FName ActionName;

    UFUNCTION(BlueprintNativeEvent, Category = "Action")
    void StartAction(AActor* InstigatorActor);

    UFUNCTION(BlueprintNativeEvent, Category = "Action")
    void StopAction(AActor* InstigatorActor);
};
