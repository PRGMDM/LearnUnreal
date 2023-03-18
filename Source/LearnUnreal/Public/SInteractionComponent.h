// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SInteractionComponent.generated.h"

class USWorldUserWidget;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class LEARNUNREAL_API USInteractionComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    USInteractionComponent();

    void PrimaryInteract();

    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
    UPROPERTY() // For GC
    AActor* FocusedActor;

    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<USWorldUserWidget> DefaultWidgetClass;

    UPROPERTY()
    TObjectPtr<USWorldUserWidget> DefaultWidgetInstance;

    UPROPERTY(EditDefaultsOnly, Category = "Trace")
    float TraceDistance = 500.f;

    UPROPERTY(EditDefaultsOnly, Category = "Trace")
    float TraceRadius = 30.f;

    UPROPERTY(EditDefaultsOnly, Category = "Trace")
    TEnumAsByte<ECollisionChannel> CollisionChannel = ECC_WorldDynamic;

    void FindBestInteractable();
};
