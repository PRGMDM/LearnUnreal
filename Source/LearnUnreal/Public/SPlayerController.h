// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/PlayerController.h"
#include "SPlayerController.generated.h"

class UInputAction;
class UInputMappingContext;

UCLASS()
class LEARNUNREAL_API ASPlayerController : public APlayerController
{
    GENERATED_BODY()

protected:
    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<UUserWidget> PauseMenuClass;

    TObjectPtr<UUserWidget> PauseMenuInstance;

    UPROPERTY(EditDefaultsOnly)
    TObjectPtr<UInputAction> PauseAction;

    UFUNCTION(BlueprintCallable)
    void TogglePauseMenu();

    void SetupInputComponent() override;
};
