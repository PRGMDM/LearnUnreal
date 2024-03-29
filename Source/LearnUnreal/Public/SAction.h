// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/NoExportTypes.h"
#include "SAction.generated.h"

class UWorld;
class USActionComponent;

USTRUCT()
struct FActionRepData
{
    GENERATED_BODY();

public:
    UPROPERTY()
    bool bIsRunning;

    UPROPERTY()
    TObjectPtr<AActor> Instigator;
};

UCLASS(Blueprintable)
class LEARNUNREAL_API USAction : public UObject
{
    GENERATED_BODY()

public:
    UPROPERTY(EditDefaultsOnly, Category = "Action")
    FName ActionName;

    UPROPERTY(EditDefaultsOnly, Category = "Action")
    bool bAutoStart = false;

    UFUNCTION(BlueprintNativeEvent, Category = "Action")
    void StartAction(AActor* InstigatorActor);

    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Action")
    void StopAction(AActor* InstigatorActor);

    UFUNCTION(BlueprintNativeEvent, Category = "Action")
    bool CanStart(AActor* InstigatorActor);

    UFUNCTION(BlueprintCallable, Category = "Action")
    bool IsRunning() const;

    UWorld* GetWorld() const override;

    bool IsSupportedForNetworking() const override { return true; }

protected:
    UPROPERTY(ReplicatedUsing = "OnRep_RepData")
    FActionRepData RepData;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
    TSoftObjectPtr<UTexture2D> Icon;

    UPROPERTY(Replicated)
    float TimeStarted;

    UFUNCTION()
    void OnRep_RepData();

    UPROPERTY(EditDefaultsOnly, Category = "Tags")
    FGameplayTagContainer GrantsTags;

    UPROPERTY(EditDefaultsOnly, Category = "Tags")
    FGameplayTagContainer BlockedTags;

    UFUNCTION(BlueprintCallable, Category = "Action")
    USActionComponent* GetOwningComponent() const;
};
