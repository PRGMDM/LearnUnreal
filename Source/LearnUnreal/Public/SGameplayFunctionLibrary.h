// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "SGameplayFunctionLibrary.generated.h"

UCLASS()
class LEARNUNREAL_API USGameplayFunctionLibrary : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()
public:
    UFUNCTION(BlueprintCallable, Category = "Gameplay")
    static bool ApplyDamage(AActor* InstigatorActor, AActor* Target, float DamageAmount);

    UFUNCTION(BlueprintCallable, Category = "Gameplay")
    static bool ApplyDirectionalDamage(AActor* InstigatorActor, AActor* Target, float DamageAmount, const FHitResult& HitResult);
};
