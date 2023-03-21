// Fill out your copyright notice in the Description page of Project Settings.

#include "SGameModeBase.h"
#include "AI/SAICharacter.h"
#include "DrawDebugHelpers.h"
#include "EngineUtils.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "Math/UnrealMathUtility.h"
#include "SAttributeComponent.h"
#include "SCharacter.h"
#include "SPlayerState.h"

static TAutoConsoleVariable<bool> CVarSpawnBots(TEXT("SpawnBots"), true, TEXT("Enable/disable bots spawning"), ECVF_Cheat);

ASGameModeBase::ASGameModeBase()
{
    SpawnTimerInterval = 2.0f;
}

void ASGameModeBase::StartPlay()
{
    Super::StartPlay();
    GetWorldTimerManager().SetTimer(TimerHandle_SpawnBots, this, &ASGameModeBase::SpawnBotTimerElapsed, SpawnTimerInterval, true);
    // TODO: A better query in editor, RandomXPct returns one result. How to find random locations within a fixed area (specified by location?)
    UEnvQueryInstanceBlueprintWrapper* QueryInstance = UEnvQueryManager::RunEQSQuery(this, SpawnItemsQuery, this, EEnvQueryRunMode::RandomBest25Pct, nullptr);
    if (QueryInstance)
    {
        QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &ASGameModeBase::OnSpawnItemsQueryComplete);
    }
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

void ASGameModeBase::OnActorKilled(AActor* Victim, AActor* Killer)
{
    UE_LOG(LogTemp, Log, TEXT("OnActorKilled: victim: %s, killer: %s"), *GetNameSafe(Victim), *GetNameSafe(Killer));

    // Respawn if player is killed
    ASCharacter* Player = Cast<ASCharacter>(Victim);
    if (Player)
    {
        FTimerHandle TimerHandle_RespawnDelay;
        FTimerDelegate Delegate;
        Delegate.BindUFunction(this, "RespawnPlayerElapsed", Player->GetController());
        float RespawnDelay = 2.f;
        GetWorldTimerManager().SetTimer(TimerHandle_RespawnDelay, Delegate, RespawnDelay, false);
    }

    // Give credits if player killed someone
    Player = Cast<ASCharacter>(Killer);
    if (Player)
    {
        ASPlayerState* State = Player->GetPlayerState<ASPlayerState>();
        if (ensure(State))
        {
            State->ModifyCredit(RewardForKill);
        }
    }
}

void ASGameModeBase::SpawnBotTimerElapsed()
{
    if (!CVarSpawnBots.GetValueOnGameThread())
    {
        UE_LOG(LogTemp, Log, TEXT("Stopped bots spawning."));
        return;
    }

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
            QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &ASGameModeBase::OnSpawnBotsQueryComplete);
        }
    }
}

void ASGameModeBase::OnSpawnBotsQueryComplete(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus)
{
    if (QueryStatus == EEnvQueryStatus::Success)
    {
        TArray<FVector> Locations = QueryInstance->GetResultsAsLocations();
        if (Locations.Num() > 0)
        {
            DrawDebugSphere(GetWorld(), Locations[0], 100, 10, FColor::Black, true, 10, 0, 5);
            GetWorld()->SpawnActor<AActor>(MinionClass, Locations[0], FRotator::ZeroRotator);
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Failed to spawn bots."));
        return;
    }
}

void ASGameModeBase::OnSpawnItemsQueryComplete(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus)
{
    if (QueryStatus == EEnvQueryStatus::Success)
    {
        TArray<FVector> Locations = QueryInstance->GetResultsAsLocations();
        for (FVector Location : Locations)
        {
            GetWorld()->SpawnActor<AActor>(ItemClasses[FMath::RandRange(0, Locations.Num() - 1)], Location, FRotator::ZeroRotator);
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Failed to spawn items."));
        return;
    }
}

void ASGameModeBase::RespawnPlayerElapsed(AController* Controller)
{
    if (ensure(Controller))
    {
        Controller->UnPossess();
        RestartPlayer(Controller);
    }
}
