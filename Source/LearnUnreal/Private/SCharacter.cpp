// Fill out your copyright notice in the Description page of Project Settings.


#include "SCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "SInteractionComponent.h"
#include "SAttributeComponent.h"
#include "Components/CapsuleComponent.h"

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

	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationYaw = false;
}

// Called when the game starts or when spawned
void ASCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void ASCharacter::MoveForward(float Value)
{
  FRotator ControlRot = GetControlRotation();
  ControlRot.Pitch = 0.f;
  ControlRot.Roll = 0.f;

  AddMovementInput(ControlRot.Vector(), Value);
}

void ASCharacter::MoveRight(float Value)
{
  FRotator ControlRot = GetControlRotation();
  ControlRot.Pitch = 0.f;
  ControlRot.Roll = 0.f;

  // X is forward, Y is right, Z is up
  FVector RightVector = FRotationMatrix(ControlRot).GetScaledAxis(EAxis::Y);
  AddMovementInput(RightVector, Value);
}

void ASCharacter::PrimaryAttack()
{
  PlayAnimMontage(AttackAnim);
  GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAttack, this, &ASCharacter::PrimaryAttack_TimeElapsed, 0.2f);
  // GetWorldTimerManager().ClearTimer(handle) stops the timer from running when character dies etc.
  // TODO: use animation notify is better, will get to this later.
}

void ASCharacter::PrimaryAttack_TimeElapsed()
{
  // The muzzle on the right hand
  FVector HandLocation = GetMesh()->GetSocketLocation("Muzzle_01");
  FVector Start = CameraComp->GetComponentLocation();
  FVector End = Start + GetControlRotation().Vector() * 10000;
  FCollisionObjectQueryParams ObjectQueryParams;
  ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldStatic);  // TODO: think about what other types to add.
  ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);

  FHitResult Hit;
  GetWorld()->LineTraceSingleByObjectType(Hit, Start, End, ObjectQueryParams);

  FVector ProjectileDirection = Hit.GetActor()? Hit.ImpactPoint - HandLocation : End - HandLocation;

  FTransform SpawnTM = FTransform(ProjectileDirection.Rotation(), HandLocation);
  FActorSpawnParameters SpawnParams;
  SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
  SpawnParams.Owner= this;
  AActor* projectile = GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnTM, SpawnParams);
  GetCapsuleComponent()->IgnoreActorWhenMoving(projectile, true);
}



void ASCharacter::PrimaryInteract()
{
  InteractionComp->PrimaryInteract();
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
	PlayerInputComponent->BindAxis("MoveForward", this, &ASCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASCharacter::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ASCharacter::Jump);
	PlayerInputComponent->BindAction("PrimaryAttack", IE_Pressed, this, &ASCharacter::PrimaryAttack);
	PlayerInputComponent->BindAction("PrimaryInteract", IE_Pressed, this, &ASCharacter::PrimaryInteract);
}

