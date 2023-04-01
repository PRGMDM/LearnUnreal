// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"
#include "SActionComponent.generated.h"

class USAction;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class LEARNUNREAL_API USActionComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    USActionComponent();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tags")
    FGameplayTagContainer ActiveGameplayTags;

    UFUNCTION(BlueprintCallable, Category = "Actions")
    void AddAction(TSubclassOf<USAction> ActionClass, AActor* InstigatorActor);

    UFUNCTION(BlueprintCallable, Category = "Actions")
    void RemoveAction(USAction* Action);

    UFUNCTION(BlueprintCallable, Category = "Actions")
    bool StartActionByName(AActor* InstigatorActor, FName ActionName);

    UFUNCTION(BlueprintCallable, Category = "Actions")
    bool StopActionByName(AActor* InstigatorActor, FName ActionName);

    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
    UPROPERTY(Replicated)
    TArray<USAction*> Actions;

    UPROPERTY(EditAnywhere, Category = "Actions")
    TArray<TSubclassOf<USAction>> DefaultActions;

    virtual void BeginPlay() override;

    UFUNCTION(Server, Reliable)
    void ServerStartAction(AActor* Instigator, FName ActionName);
};
