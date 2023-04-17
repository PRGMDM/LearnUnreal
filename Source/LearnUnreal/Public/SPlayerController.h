// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/PlayerController.h"
#include "SPlayerController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlayerStateReplication);

class UInputAction;

UCLASS()
class LEARNUNREAL_API ASPlayerController : public APlayerController
{
    GENERATED_BODY()

    void OnRep_PlayerState() override;

protected:
    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<UUserWidget> PauseMenuClass;

    TObjectPtr<UUserWidget> PauseMenuInstance;

    bool bPlayerStateReady = false;

    UPROPERTY(BlueprintAssignable)
    FOnPlayerStateReplication OnPlayerStateReplication;

    UPROPERTY(EditDefaultsOnly)
    TObjectPtr<UInputAction> PauseAction;

    UFUNCTION(BlueprintCallable)
    void TogglePauseMenu();

    void SetupInputComponent() override;
};
