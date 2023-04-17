// Fill out your copyright notice in the Description page of Project Settings.

#include "SGameModeBase.h"
#include "AI/SAICharacter.h"
#include "DrawDebugHelpers.h"
#include "Engine/AssetManager.h"
#include "EngineUtils.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "GameFramework/GameState.h"
#include "Kismet/GameplayStatics.h"
#include "Math/UnrealMathUtility.h"
#include "SActionComponent.h"
#include "SAttributeComponent.h"
#include "SCharacter.h"
#include "SEnemyData.h"
#include "SGameplayInterface.h"
#include "SPlayerState.h"
#include "SSaveGame.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"

static TAutoConsoleVariable<bool> CVarSpawnBots(TEXT("SpawnBots"), false, TEXT("Enable/disable bots spawning"), ECVF_Cheat);

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
            // DrawDebugSphere(GetWorld(), Locations[0], 100, 10, FColor::Black, true, 10, 0, 5);

            TArray<FEnemyInfoRow*> Rows;
            EnemyTable->GetAllRows("", Rows);
            FEnemyInfoRow* Selected = Rows[FMath::RandRange(0, Rows.Num() - 1)];

            TObjectPtr<UAssetManager> Manager = UAssetManager::GetIfValid();
            if (Manager)
            {
                TArray<FName> Bundles;

                FStreamableDelegate Delegate = FStreamableDelegate::CreateUObject(this, &ASGameModeBase::OnEnemyLoaded, Selected->EnemyId, Locations[0]);

                Manager->LoadPrimaryAsset(Selected->EnemyId, Bundles, Delegate);
            }
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Failed to spawn bots."));
        return;
    }
}

void ASGameModeBase::OnEnemyLoaded(FPrimaryAssetId Id, FVector SpawnLocation)
{

    TObjectPtr<UAssetManager> Manager = UAssetManager::GetIfValid();
    if (Manager)
    {
        TObjectPtr<USEnemyData> Data = Cast<USEnemyData>(Manager->GetPrimaryAssetObject(Id));
        if (Data)
        {
            TObjectPtr<AActor> NewBot = GetWorld()->SpawnActor<AActor>(Data->EnemyClass, SpawnLocation, FRotator::ZeroRotator);
            if (NewBot)
            {
                TObjectPtr<USActionComponent> ActionComp = Cast<USActionComponent>(NewBot->GetComponentByClass(USActionComponent::StaticClass()));
                if (ActionComp)
                {
                    for (auto ActionClass : Data->Actions)
                    {
                        ActionComp->AddAction(ActionClass, NewBot);
                    }
                }
            }
        }
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

void ASGameModeBase::WriteSaveGame()
{
    UE_LOG(LogTemp, Log, TEXT("Saving game."));
    for (TObjectPtr<APlayerState> State : GameState->PlayerArray)
    {
        ASPlayerState* PS = Cast<ASPlayerState>(State);
        PS->SavePlayerState(CurrentSaveGame);
        break;
    }

    CurrentSaveGame->SavedActors.Empty();

    for (FActorIterator It(GetWorld()); It; ++It)
    {
        AActor* Actor = *It;
        if (Actor->Implements<USGameplayInterface>())
        {
            FActorSaveData Data;
            Data.ActorName = Actor->GetName();
            Data.Transform = Actor->GetTransform();

            FMemoryWriter MemWriter(Data.ByteData);
            FObjectAndNameAsStringProxyArchive Ar(MemWriter, true);
            Ar.ArIsSaveGame = true;
            Actor->Serialize(Ar);

            CurrentSaveGame->SavedActors.Add(Data);
        }
    }

    UGameplayStatics::SaveGameToSlot(CurrentSaveGame, SlotName, 0);
}

void ASGameModeBase::LoadSaveGame()
{
    if (UGameplayStatics::DoesSaveGameExist(SlotName, 0))
    {
        CurrentSaveGame = Cast<USSaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName, 0));
        if (CurrentSaveGame == nullptr)
        {
            UE_LOG(LogTemp, Warning, TEXT("Failed to load saved game."));
            return;
        }
        UE_LOG(LogTemp, Log, TEXT("Loaded saved game."));

        for (FActorIterator It(GetWorld()); It; ++It)
        {
            AActor* Actor = *It;
            if (Actor->Implements<USGameplayInterface>())
            {
                for (FActorSaveData Data : CurrentSaveGame->SavedActors)
                {
                    if (Data.ActorName == Actor->GetName())
                    {
                        Actor->SetActorTransform(Data.Transform);

                        FMemoryReader MemReader(Data.ByteData);

                        FObjectAndNameAsStringProxyArchive Ar(MemReader, true);
                        Ar.ArIsSaveGame = true;
                        Actor->Serialize(Ar);
                        ISGameplayInterface::Execute_OnActorLoaded(Actor);

                        break;
                    }
                }
            }
        }
    }
    else
    {
        CurrentSaveGame = Cast<USSaveGame>(UGameplayStatics::CreateSaveGameObject(USSaveGame::StaticClass()));
        UE_LOG(LogTemp, Log, TEXT("Created new save data"));
    }
}

void ASGameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
    Super::InitGame(MapName, Options, ErrorMessage);

    FString SelectedSlot = UGameplayStatics::ParseOption(Options, "SaveGame");
    if (!SelectedSlot.IsEmpty())
    {
        SlotName = SelectedSlot;
    }

    LoadSaveGame();
}

void ASGameModeBase::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
    TObjectPtr<ASPlayerState> State = NewPlayer->GetPlayerState<ASPlayerState>();
    if (State)
    {
        State->LoadPlayerState(CurrentSaveGame);
    }

    Super::HandleStartingNewPlayer_Implementation(NewPlayer);
}
