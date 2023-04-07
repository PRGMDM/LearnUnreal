// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "SPlayerState.generated.h"

class USSaveGame;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCreditsChanged);

UCLASS()
class LEARNUNREAL_API ASPlayerState : public APlayerState
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "Credits")
    bool ModifyCredit(int32 Amount);

    int32 GetCredits() const;

    UFUNCTION(BlueprintNativeEvent)
    void SavePlayerState(USSaveGame* SaveGame);

    UFUNCTION(BlueprintNativeEvent)
    void LoadPlayerState(USSaveGame* SaveGame);

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Credits")
    int32 Credits;

    UPROPERTY(BlueprintAssignable)
    FOnCreditsChanged OnCreditsChanged;
};
