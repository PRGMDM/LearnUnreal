// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

DECLARE_STATS_GROUP(TEXT("My custom stats"), STATGROUP_CUSTOM, STATCAT_Advanced);

static void LogOnScreen(UObject* WorldContext, FString Msg, FColor Color = FColor::White, float Duration = 5.f)
{
    if (!ensure(WorldContext))
    {
        return;
    }

    UWorld* World = WorldContext->GetWorld();
    if (!ensure(World))
    {
        return;
    }

    FString NetPrefix = World->IsNetMode(NM_Client) ? "[Client] " : "[Server] ";
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, Duration, Color, NetPrefix + Msg);
    }
}
