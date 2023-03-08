// Fill out your copyright notice in the Description page of Project Settings.

#include "SCharacter.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "SAction.h"
#include "SActionComponent.h"
#include "SAttributeComponent.h"
#include "SInteractionComponent.h"

// Sets default values
ASCharacter::ASCharacter()
{
    // Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    SpringArmComp = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
    SpringArmComp->bUsePawnControlRotation = true;
    SpringArmComp->SetupAttachment(RootComponent);

    CameraComp = CreateDefaultSubobject<UCameraComponent>("CameraComp");
    CameraComp->SetupAttachment(SpringArmComp);

    InteractionComp = CreateDefaultSubobject<USInteractionComponent>("InteractionComp");

    AttributeComp = CreateDefaultSubobject<USAttributeComponent>("AttributeComponent");

    ActionComp = CreateDefaultSubobject<USActionComponent>("ActionComponent");

    GetCharacterMovement()->bOrientRotationToMovement = true;
    bUseControllerRotationYaw = false;
}

void ASCharacter::HealSelf(float Amount /* = 100.f */)
{
    AttributeComp->ApplyHealthChange(this, Amount);
}

void ASCharacter::PostInitializeComponents()
{
    Super::PostInitializeComponents();
    AttributeComp->OnHealthChanged.AddDynamic(this, &ASCharacter::OnHealthChanged);
}

FVector ASCharacter::GetPawnViewLocation() const
{
    return CameraComp->GetComponentLocation();
}

// Called when the game starts or when spawned
void ASCharacter::BeginPlay()
{
    Super::BeginPlay();
    APlayerController* PC = Cast<APlayerController>(GetController());
    if (PC)
    {
        UEnhancedInputLocalPlayerSubsystem* InputSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer());
        if (InputSystem)
        {
            ensure(InputMapping);
            InputSystem->AddMappingContext(InputMapping, 0);
        }
    }
}

void ASCharacter::Move(const FInputActionInstance& Instance)
{
    FVector2D Input = Instance.GetValue().Get<FVector2D>();
    FRotator ControlRot = GetControlRotation();
    ControlRot.Pitch = 0.f;
    ControlRot.Roll = 0.f;

    FVector RightVector = FRotationMatrix(ControlRot).GetScaledAxis(EAxis::Y);

    AddMovementInput(ControlRot.Vector(), Input[0]);
    AddMovementInput(RightVector, Input[1]);
}

void ASCharacter::Look(const FInputActionInstance& Instance)
{
    AddControllerYawInput(Instance.GetValue().Get<FVector2D>()[0]);
    AddControllerPitchInput(-Instance.GetValue().Get<FVector2D>()[1]);
}

void ASCharacter::SprintStart()
{
    ActionComp->StartActionByName(this, "Sprint");
}

void ASCharacter::SprintStop()
{
    ActionComp->StopActionByName(this, "Sprint");
}

void ASCharacter::PrimaryAttack()
{
    ActionComp->StartActionByName(this, "PrimaryAttack");
}

void ASCharacter::BlackHoleAttack()
{
    ActionComp->StartActionByName(this, "SecondaryAttack");
}

void ASCharacter::Dash()
{
    ActionComp->StartActionByName(this, "Dash");
}

void ASCharacter::PrimaryInteract()
{
    InteractionComp->PrimaryInteract();
}

void ASCharacter::OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta)
{
    if (Delta < 0)
    {
        GetMesh()->SetScalarParameterValueOnMaterials("TimeToHit", GetWorld()->TimeSeconds);
        if (NewHealth <= 0)
        {
            APlayerController* PC = Cast<APlayerController>(GetController());
            DisableInput(PC);
        }
    }
}

// Called every frame
void ASCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ASCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
    UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(PlayerInputComponent);

    Input->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ASCharacter::Move);
    Input->BindAction(LookAction, ETriggerEvent::Triggered, this, &ASCharacter::Look);
    Input->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ASCharacter::Jump);
    Input->BindAction(PrimaryAttackAction, ETriggerEvent::Completed, this, &ASCharacter::PrimaryAttack);
    Input->BindAction(SecondaryAttackAction, ETriggerEvent::Completed, this, &ASCharacter::BlackHoleAttack);
    Input->BindAction(InteractAction, ETriggerEvent::Completed, this, &ASCharacter::PrimaryInteract);
    Input->BindAction(DashAction, ETriggerEvent::Completed, this, &ASCharacter::Dash);
    Input->BindAction(SprintAction, ETriggerEvent::Started, this, &ASCharacter::SprintStart);
    Input->BindAction(SprintAction, ETriggerEvent::Completed, this, &ASCharacter::SprintStop);
}
