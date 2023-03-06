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
class UParticleSystem;
class UInputAction;
class UInputMappingContext;

UCLASS()
class LEARNUNREAL_API ASCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    // Sets default values for this character's properties
    ASCharacter();

    UFUNCTION(Exec)
    void HealSelf(float Amount = 100.f);

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

    UPROPERTY(EditAnywhere, Category = "Effects")
    UParticleSystem* AttackVFX;

    UPROPERTY(VisibleAnywhere, Category = "Effects")
    FName HandSocketName;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    USAttributeComponent* AttributeComp;

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    UInputMappingContext* InputMapping;

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    UInputAction* MoveAction;

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    UInputAction* LookAction;

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    UInputAction* JumpAction;

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    UInputAction* PrimaryAttackAction;

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    UInputAction* SecondaryAttackAction;

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    UInputAction* InteractAction;

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    UInputAction* DashAction;

    FTimerHandle TimerHandle_PrimaryAttack;
    FTimerHandle TimerHandle_BlackHoleAttack;
    FTimerHandle TimerHandle_Dash;

    UFUNCTION()
    void Move(const FInputActionInstance& Instance);

    UFUNCTION()
    void Look(const FInputActionInstance& Instance);

    void PrimaryAttack();

    void PrimaryAttack_TimeElapsed();

    void PrimaryInteract();

    void BlackHoleAttack();

    void BlackHoleAttack_TimeElapsed();

    void Dash();

    void Dash_TimeElapsed();

    void SpawnProjectile(TSubclassOf<AActor> ClassToSpawn);

    void PlayAttackEffects();

    UFUNCTION()
    void OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta);

    virtual void PostInitializeComponents();

public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;

    // Called to bind functionality to input
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
