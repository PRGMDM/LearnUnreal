// Fill out your copyright notice in the Description page of Project Settings.

#include "SCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
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

    HandSocketName = "Muzzle_01";
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
    UE_LOG(LogTemp, Log, TEXT("Sprinting started"));
    ActionComp->StartActionByName(this, "Sprint");
}

void ASCharacter::SprintStop()
{
    UE_LOG(LogTemp, Log, TEXT("Sprinting stoped"));

    ActionComp->StopActionByName(this, "Sprint");
}

void ASCharacter::PlayAttackEffects()
{
    PlayAnimMontage(AttackAnim);
    UGameplayStatics::SpawnEmitterAttached(AttackVFX, GetMesh(), HandSocketName, FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::SnapToTarget);
}

void ASCharacter::PrimaryAttack()
{
    PlayAttackEffects();
    GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAttack, this, &ASCharacter::PrimaryAttack_TimeElapsed, 0.2f);
    // GetWorldTimerManager().ClearTimer(handle) stops the timer from running when character dies etc.
    // TODO: use animation notify is better, will get to this later.
}

void ASCharacter::PrimaryAttack_TimeElapsed()
{
    SpawnProjectile(ProjectileClass);
}

void ASCharacter::BlackHoleAttack()
{
    PlayAttackEffects();

    GetWorldTimerManager().SetTimer(TimerHandle_BlackHoleAttack, this, &ASCharacter::BlackHoleAttack_TimeElapsed, 0.2f);
}

void ASCharacter::BlackHoleAttack_TimeElapsed()
{
    SpawnProjectile(BlackHoleClass);
}

void ASCharacter::Dash()
{
    PlayAttackEffects();

    GetWorldTimerManager().SetTimer(TimerHandle_Dash, this, &ASCharacter::Dash_TimeElapsed, 0.2f);
}

void ASCharacter::Dash_TimeElapsed()
{
    SpawnProjectile(DashProjectileClass);
}

void ASCharacter::PrimaryInteract()
{
    InteractionComp->PrimaryInteract();
}

void ASCharacter::SpawnProjectile(TSubclassOf<AActor> ClassToSpawn)
{
    if (ensureAlways(ClassToSpawn))
    {
        // The muzzle on the right hand of Gideon.
        FVector HandLocation = GetMesh()->GetSocketLocation(HandSocketName);

        FCollisionShape Shape;
        Shape.SetSphere(20.f);

        FCollisionQueryParams Params;
        Params.AddIgnoredActor(this);

        FCollisionObjectQueryParams ObjParams;
        ObjParams.AddObjectTypesToQuery(ECC_WorldStatic); // TODO: When look above, the camera could be in the hitbox of landscape, what to do? make the trace start further away.
        ObjParams.AddObjectTypesToQuery(ECC_WorldDynamic);
        ObjParams.AddObjectTypesToQuery(ECC_Pawn);

        FVector Start = CameraComp->GetComponentLocation();
        FVector End = Start + GetControlRotation().Vector() * 10000;

        FHitResult Hit;
        if (GetWorld()->SweepSingleByObjectType(Hit, Start, End, FQuat::Identity, ObjParams, Shape, Params))
        {
            End = Hit.ImpactPoint;
        }

        FRotator ProjRotation = FRotationMatrix::MakeFromX(End - HandLocation).Rotator();
        FTransform SpawnTM = FTransform(ProjRotation, HandLocation);
        FActorSpawnParameters SpawnParams;
        SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
        SpawnParams.Instigator = this;
        AActor* Projectile = GetWorld()->SpawnActor<AActor>(ClassToSpawn, SpawnTM, SpawnParams);
        GetCapsuleComponent()->IgnoreActorWhenMoving(Projectile, true);
    }
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
    Input->BindAction(PrimaryAttackAction, ETriggerEvent::Triggered, this, &ASCharacter::PrimaryAttack);
    Input->BindAction(SecondaryAttackAction, ETriggerEvent::Triggered, this, &ASCharacter::BlackHoleAttack);
    Input->BindAction(InteractAction, ETriggerEvent::Triggered, this, &ASCharacter::PrimaryInteract);
    Input->BindAction(DashAction, ETriggerEvent::Triggered, this, &ASCharacter::Dash);
    Input->BindAction(SprintAction, ETriggerEvent::Started, this, &ASCharacter::SprintStart);
    Input->BindAction(SprintAction, ETriggerEvent::Completed, this, &ASCharacter::SprintStop);
}
