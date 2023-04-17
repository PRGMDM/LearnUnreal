// Fill out your copyright notice in the Description page of Project Settings.

#include "SPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

void ASPlayerController::OnRep_PlayerState()
{
    Super::OnRep_PlayerState();
    if (!bPlayerStateReady)
    {
        OnPlayerStateReplication.Broadcast();
    }
}

void ASPlayerController::TogglePauseMenu()
{
    ensure(PauseMenuClass);
    if (PauseMenuInstance && PauseMenuInstance->IsInViewport())
    {
        UE_LOG(LogTemp, Log, TEXT("Leaving pause menu"));
        PauseMenuInstance->RemoveFromParent();
        PauseMenuInstance = nullptr;
        bShowMouseCursor = false;
        SetInputMode(FInputModeGameOnly());
    }
    else
    {
        UE_LOG(LogTemp, Log, TEXT("Entering pause menu"));
        PauseMenuInstance = CreateWidget<UUserWidget>(this, PauseMenuClass);
        ensure(PauseMenuInstance);
        PauseMenuInstance->AddToViewport();
        bShowMouseCursor = true;
        SetInputMode(FInputModeUIOnly());
    }
}

void ASPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();
    TObjectPtr<UEnhancedInputComponent> Input = Cast<UEnhancedInputComponent>(InputComponent);
    Input->BindAction(PauseAction, ETriggerEvent::Completed, this, &ASPlayerController::TogglePauseMenu);
}
