// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SAICharacter.generated.h"

class UPawnSensingComponent;
class USAttributeComponent;
class UUserWidget;
class USWorldUserWidget;
class USActionComponent;

UCLASS()
class LEARNUNREAL_API ASAICharacter : public ACharacter
{
    GENERATED_BODY()

public:
    ASAICharacter();

protected:
    UPROPERTY(VisibleAnywhere, Category = "Components")
    UPawnSensingComponent* PawnSensingComp;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    USAttributeComponent* AttributeComp;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    USActionComponent* ActionComp;

    UPROPERTY(VisibleAnywhere, Category = "Effect")
    FName TimeToHitParamName;

    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<UUserWidget> HealthBarWidgetClass;

    USWorldUserWidget* ActiveHealthBar;

    virtual void PostInitializeComponents() override;

    void SetTargetActor(AActor* Target);

    UFUNCTION()
    void OnPawnSeen(APawn* Pawn);

    UFUNCTION()
    void OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta);
};
