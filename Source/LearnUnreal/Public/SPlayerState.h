// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "SPlayerState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCreditsChanged);

UCLASS()
class LEARNUNREAL_API ASPlayerState : public APlayerState
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "Credits")
    bool ModifyCredit(float Amount);

    float GetCredits() const;

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Credits")
    float Credits;

    UPROPERTY(BlueprintAssignable)
    FOnCreditsChanged OnCreditsChanged;
};
