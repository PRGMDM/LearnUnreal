// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "GameFramework/GameModeBase.h"
#include "SPowerupItemBase.h"
#include "SGameModeBase.generated.h"

class UEnvQuery;
class UEnvQueryInstanceBlueprintWrapper;
class UCurveFloat;

UCLASS()
class LEARNUNREAL_API ASGameModeBase : public AGameModeBase
{
    GENERATED_BODY()
public:
    ASGameModeBase();
    virtual void StartPlay() override;

    UFUNCTION(Exec)
    void KillAll();

    virtual void OnActorKilled(AActor* Victim, AActor* Killer);

protected:
    FTimerHandle TimerHandle_SpawnBots;

    UPROPERTY(EditDefaultsOnly, Category = "AI")
    float SpawnTimerInterval;

    UPROPERTY(EditDefaultsOnly, Category = "AI")
    UEnvQuery* SpawnBotQuery;

    UPROPERTY(EditDefaultsOnly, Category = "AI")
    TSubclassOf<AActor> MinionClass;

    UPROPERTY(EditDefaultsOnly, Category = "AI")
    UCurveFloat* DifficultyCurve;

    UPROPERTY(EditDefaultsOnly, Category = "Items")
    UEnvQuery* SpawnItemsQuery;

    UPROPERTY(EditDefaultsOnly, Category = "Credits")
    float RewardForKill;

    UPROPERTY(EditDefaultsOnly, Category = "Items")
    TArray<TSubclassOf<ASPowerupItemBase>> ItemClasses;

    UFUNCTION()
    void SpawnBotTimerElapsed();

    UFUNCTION()
    void OnSpawnBotsQueryComplete(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);

    UFUNCTION()
    void OnSpawnItemsQueryComplete(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);

    UFUNCTION()
    void RespawnPlayerElapsed(AController* Controller);
};
