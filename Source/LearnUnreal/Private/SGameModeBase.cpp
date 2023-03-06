// Fill out your copyright notice in the Description page of Project Settings.

#include "SGameModeBase.h"
#include "AI/SAICharacter.h"
#include "DrawDebugHelpers.h"
#include "EngineUtils.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "SAttributeComponent.h"

ASGameModeBase::ASGameModeBase()
{
    SpawnTimerInterval = 2.0f;
}

void ASGameModeBase::StartPlay()
{
    Super::StartPlay();
    GetWorldTimerManager().SetTimer(TimerHandle_SpawnBots, this, &ASGameModeBase::SpawnBotTimerElapsed, SpawnTimerInterval, true);
}

void ASGameModeBase::KillAll()
{
    for (TActorIterator<ASAICharacter> It(GetWorld()); It; ++It)
    {
        ASAICharacter* Bot = *It;
        if (Bot && USAttributeComponent::IsActorAlive(Bot))
        {
            USAttributeComponent::GetAttributes(Bot)->Kill(this);
        }
    }
}

void ASGameModeBase::SpawnBotTimerElapsed()
{
    int32 AliveBotsCount = 0;
    for (TActorIterator<ASAICharacter> It(GetWorld()); It; ++It)
    {
        ASAICharacter* Bot = *It;
        if (Bot && USAttributeComponent::IsActorAlive(Bot))
        {
            AliveBotsCount++;
        }
    }

    UE_LOG(LogTemp, Log, TEXT("Alive bots: %d"), AliveBotsCount);

    float MaxBotsCount = 5;
    if (ensure(DifficultyCurve))
    {
        MaxBotsCount = DifficultyCurve->GetFloatValue(GetWorld()->TimeSeconds);
    }

    if (AliveBotsCount < MaxBotsCount)
    {
        UEnvQueryInstanceBlueprintWrapper* QueryInstance = UEnvQueryManager::RunEQSQuery(this, SpawnBotQuery, this, EEnvQueryRunMode::RandomBest5Pct, nullptr);
        if (QueryInstance)
        {
            QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &ASGameModeBase::OnQueryComplete);
        }
    }
}

void ASGameModeBase::OnQueryComplete(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus)
{
    if (QueryStatus == EEnvQueryStatus::Success)
    {
        TArray<FVector> Locations = QueryInstance->GetResultsAsLocations();
        if (Locations.Num() > 0)
        {
            DrawDebugSphere(GetWorld(), Locations[0], 100, 10, FColor::Black, true, 10, 0, 5);
            // FActorSpawnParameters Params;
            // Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
            GetWorld()->SpawnActor<AActor>(MinionClass, Locations[0], FRotator::ZeroRotator);
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Failed to spawn bots."));
        return;
    }
}
