// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SAttributeComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnHealthChanged, AActor*, InstigatorActor, USAttributeComponent*, OwningComp, float, NewHealth, float, Delta);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnRageChanged, USAttributeComponent*, OwningComp, float, NewRage);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class LEARNUNREAL_API USAttributeComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    // Sets default values for this component's properties
    USAttributeComponent();

    UPROPERTY(BlueprintAssignable)
    FOnHealthChanged OnHealthChanged;

    UPROPERTY(BlueprintAssignable)
    FOnRageChanged OnRageChanged;

    UFUNCTION(BlueprintCallable, Category = "Attributes")
    bool ApplyHealthChange(AActor* InstigatorActor, float Delta);

    UFUNCTION(BlueprintCallable, Category = "Attributes")
    bool ApplyRageChange(float Delta);

    UFUNCTION(BlueprintCallable)
    bool IsAlive() const;

    UFUNCTION(BlueprintCallable)
    bool IsFullHealth() const;

    float GetHealth() const;

    UFUNCTION(BlueprintCallable)
    float GetHealthMax() const;

    UFUNCTION(BlueprintCallable)
    float GetRageMax() const;

    UFUNCTION(BlueprintCallable)
    bool IsLowHealth() const;

    UFUNCTION(BlueprintCallable)
    bool Kill(AActor* InstigatorActor);

    UFUNCTION(BlueprintCallable, Category = "Attributes")
    static USAttributeComponent* GetAttributes(AActor* Actor);

    UFUNCTION(BlueprintCallable, Category = "Attributes", meta = (DisplayName = "IsAlive"))
    static bool IsActorAlive(AActor* Actor);

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Replicated, Category = "Attributes")
    float Health;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Replicated, Category = "Attributes")
    float HealthMax = 100.f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Replicated, Category = "Attributes")
    float Rage = 0.f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Replicated, Category = "Attributes")
    float RageMax = 100.f;

    UPROPERTY(EditDefaultsOnly, Category = "Attributes")
    float DamageToRageMultiplier = 1.f;

    UFUNCTION(NetMulticast, Reliable)
    void MulticastHealthChanged(AActor* InstigatorActor, float NewHealth, float Delta);

    UFUNCTION(NetMulticast, Unreliable)
    void MuticastRageChanged(float NewRage);
};
