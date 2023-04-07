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
class USActionComponent;
struct FInputActionInstance;

UCLASS()
class LEARNUNREAL_API ASCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    // Sets default values for this character's properties
    ASCharacter();

    UFUNCTION(Exec)
    void HealSelf(float Amount = 100.f);

    UFUNCTION(Exec)
    void IncreaseRage(float Amount);

    virtual FVector GetPawnViewLocation() const override;

    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    UPROPERTY(VisibleAnywhere)
    USpringArmComponent* SpringArmComp;

    UPROPERTY(VisibleAnywhere)
    UCameraComponent* CameraComp;

    UPROPERTY(EditAnywhere)
    USInteractionComponent* InteractionComp;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    USAttributeComponent* AttributeComp;

    UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category = "Components")
    USActionComponent* ActionComp;

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

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    UInputAction* SprintAction;

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    UInputAction* ParryAction;

    UPROPERTY(EditDefaultsOnly, Category = "Ability")
    float BlackHoleRageCost = 100.f;

    void Move(const FInputActionInstance& Instance);

    void Look(const FInputActionInstance& Instance);

    void SprintStart();

    void SprintStop();

    void PrimaryAttack();

    void PrimaryInteract();

    void BlackHoleAttack();

    void Parry();

    void Dash();

    UFUNCTION()
    void OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta);

    virtual void PostInitializeComponents();
};
