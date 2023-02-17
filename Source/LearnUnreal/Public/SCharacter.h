// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class USInteractionComponent;
class UAnimMontage;
class USAttributeComponent;

UCLASS()
class LEARNUNREAL_API ASCharacter : public ACharacter {
    GENERATED_BODY()

public:
    // Sets default values for this character's properties
    ASCharacter();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    UPROPERTY(VisibleAnywhere)
    USpringArmComponent* SpringArmComp;

    UPROPERTY(VisibleAnywhere)
    UCameraComponent* CameraComp;

    UPROPERTY(EditAnywhere)
    TSubclassOf<AActor> ProjectileClass;

    UPROPERTY(EditAnywhere)
    TSubclassOf<AActor> DashProjectileClass;

    UPROPERTY(EditAnywhere)
    TSubclassOf<AActor> BlackHoleClass;

    UPROPERTY(EditAnywhere)
    USInteractionComponent* InteractionComp;

    UPROPERTY(EditAnywhere, Category = "Attack")
    UAnimMontage* AttackAnim;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    USAttributeComponent* AttributeComp;

    FTimerHandle TimerHandle_PrimaryAttack;

    FTimerHandle TimerHandle_BlackHoleAttack;
    FTimerHandle TimerHandle_Dash;

    void MoveForward(float Value);

    void MoveRight(float Value);

    void PrimaryAttack();

    void PrimaryAttack_TimeElapsed();

    void PrimaryInteract();

    void BlackHoleAttack();

    void BlackHoleAttack_TimeElapsed();

    void Dash();

    void Dash_TimeElapsed();

    void SpawnProjectile(TSubclassOf<AActor> ClassToSpawn);

    UFUNCTION()
    void OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta);

    virtual void PostInitializeComponents();

public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;

    // Called to bind functionality to input
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
