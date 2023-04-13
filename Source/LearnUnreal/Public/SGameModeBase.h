// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/Datatable.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "GameFramework/GameModeBase.h"
#include "SPowerupItemBase.h"
#include "SGameModeBase.generated.h"

class UEnvQuery;
class UEnvQueryInstanceBlueprintWrapper;
class UCurveFloat;
class USSaveGame;
class UDataTable;
class USEnemyData;

USTRUCT(BlueprintType)
struct FEnemyInfoRow : public FTableRowBase
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FPrimaryAssetId EnemyId;
};

UCLASS()
class LEARNUNREAL_API ASGameModeBase : public AGameModeBase
{
    GENERATED_BODY()

public:
    virtual void StartPlay() override;

    UFUNCTION(Exec)
    void KillAll();

    virtual void OnActorKilled(AActor* Victim, AActor* Killer);

    UFUNCTION(BlueprintCallable, Category = "SaveGame")
    void WriteSaveGame();

    void LoadSaveGame();

    virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;

    void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;

protected:
    FString SlotName = "SaveGame_00";

    UPROPERTY()
    TObjectPtr<USSaveGame> CurrentSaveGame;

    FTimerHandle TimerHandle_SpawnBots;

    UPROPERTY(EditDefaultsOnly, Category = "AI")
    TObjectPtr<UDataTable> EnemyTable;

    UPROPERTY(EditDefaultsOnly, Category = "AI")
    float SpawnTimerInterval = 2.0f;

    UPROPERTY(EditDefaultsOnly, Category = "AI")
    UEnvQuery* SpawnBotQuery;

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

    void OnEnemyLoaded(FPrimaryAssetId Id, FVector SpawnLocation);

    UFUNCTION()
    void RespawnPlayerElapsed(AController* Controller);
};
