// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/NoExportTypes.h"
#include "SAction.generated.h"

class UWorld;
class USActionComponent;

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

    UFUNCTION(BlueprintNativeEvent, Category = "Action")
    bool CanStart(AActor* Instigator);

    UFUNCTION(BlueprintCallable, Category = "Action")
    bool IsRunning() const;

    UWorld* GetWorld() const override;

protected:
    bool bIsRunning;

    UPROPERTY(EditDefaultsOnly, Category = "Tags")
    FGameplayTagContainer GrantsTags;

    UPROPERTY(EditDefaultsOnly, Category = "Tags")
    FGameplayTagContainer BlockedTags;

    UFUNCTION(BlueprintCallable, Category = "Action")
    USActionComponent* GetOwningComponent() const;
};