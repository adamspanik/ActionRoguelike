// Fill out your copyright notice in the Description page of Project Settings.


#include "SCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "DrawDebugHelpers.h"
#include "SAttributeComponent.h"
#include "SInteractionComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ASCharacter::ASCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>("SpringArmComp");
	SpringArmComp->bUsePawnControlRotation = true;
	SpringArmComp->SetupAttachment(RootComponent);

	CameraComp = CreateDefaultSubobject<UCameraComponent>("CameraComp");
	CameraComp->SetupAttachment(SpringArmComp);

	InteractionComp = CreateDefaultSubobject<USInteractionComponent>("InteractionComp");

	AttributeComp = CreateDefaultSubobject<USAttributeComponent>("AttributeComp");

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
}

void ASCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	AttributeComp->OnHealthChanged.AddDynamic(this, &ASCharacter::OnHealthChanged);
}

// Called every frame
void ASCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// -- Rotation Visualization -- //
	const float DrawScale = 100.0f;
	const float Thickness = 5.0f;

	FVector LineStart = GetActorLocation();
	// Offset to the right of pawn
	LineStart += GetActorRightVector() * 100.0f;
	// Set line end in direction of the actor's forward
	FVector ActorDirection_LineEnd = LineStart + (GetActorForwardVector() * 100.0f);
	// Draw Actor's Direction
	DrawDebugDirectionalArrow(GetWorld(), LineStart, ActorDirection_LineEnd, DrawScale, FColor::Yellow, false, 0.0f, 0, Thickness);

	FVector ControllerDirection_LineEnd = LineStart + (GetControlRotation().Vector() * 100.0f);
	// Draw 'Controller' Rotation ('PlayerController' that 'possessed' this character)
	DrawDebugDirectionalArrow(GetWorld(), LineStart, ControllerDirection_LineEnd, DrawScale, FColor::Green, false, 0.0f, 0, Thickness);
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

	PlayerInputComponent->BindAction("SpecialAttack", IE_Pressed, this, &ASCharacter::SpecialAttack);
	PlayerInputComponent->BindAction("Dash", IE_Pressed, this, &ASCharacter::Dash);
}

void ASCharacter::MoveForward(float Value)
{
	//AddMovementInput(CameraComp->GetForwardVector(), Value);
	FRotator ControlRotation = GetControlRotation();
	ControlRotation.Pitch = 0;
	ControlRotation.Roll = 0;
	
	AddMovementInput(ControlRotation.Vector(), Value);
}

void ASCharacter::MoveRight(float Value)
{
	FRotator ControlRotation = GetControlRotation();
	ControlRotation.Pitch = 0;
	ControlRotation.Roll = 0;

	FVector ControlRotationRightVector = UKismetMathLibrary::GetRightVector(ControlRotation);
	// AddMovementInput(GetActorRightVector(), Value);
	// AddMovementInput(CameraComp->GetRightVector(), Value);
	// AddMovementInput(ControlRotation.Vector().RightVector, Value);
	AddMovementInput(ControlRotationRightVector, Value);

}

void ASCharacter::PrimaryAttack_TimeElapsed(TSubclassOf<AActor> ProjectileClass)
{
	FVector HandLocation = GetMesh()->GetSocketLocation("Muzzle_01");
	FVector End = CameraComp->GetComponentLocation() + GetControlRotation().Vector() * 1000;
	FCollisionQueryParams QueryParams;
	QueryParams.bTraceComplex = false;
	
	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldStatic);
	ObjectQueryParams.AddObjectTypesToQuery(ECC_PhysicsBody);
	
	//DrawDebugLine(GetWorld(), CameraComp->GetComponentLocation(), End, FColor::Red, true);
	
	FHitResult HitResult;
	bool bHit = GetWorld()->LineTraceSingleByObjectType(HitResult, CameraComp->GetComponentLocation(), End, ObjectQueryParams, QueryParams);

	FRotator ProjectileRot;
	if (bHit)
	{
		ProjectileRot = UKismetMathLibrary::FindLookAtRotation(HandLocation, HitResult.ImpactPoint);
	}
	else
	{
		ProjectileRot = UKismetMathLibrary::FindLookAtRotation(HandLocation, HitResult.TraceEnd);
	}
	
	//FTransform SpawnTM = FTransform(GetControlRotation(), HandLocation);
	FTransform SpawnTM = FTransform(ProjectileRot, HandLocation);
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParams.Instigator = this;
	
	GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnTM, SpawnParams);
}

void ASCharacter::PrimaryAttack()
{
	PlayAnimMontage(AttackAnimation);

	FVector HandLocation = GetMesh()->GetSocketLocation("Muzzle_01");
	UGameplayStatics::SpawnEmitterAttached(ParticleSystem, GetMesh(), "Hand", HandLocation, FRotator::ZeroRotator, FVector(1), EAttachLocation::KeepWorldPosition);
	
	FTimerDelegate TimerDelegate = FTimerDelegate::CreateUObject( this, &ASCharacter::PrimaryAttack_TimeElapsed, MagicProjectileClass);
	GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAttack, TimerDelegate,  0.2f, false);
	// GetWorldTimerManager().ClearTimer(TimerHandle_PrimaryAttack);
}

void ASCharacter::Dash()
{
	PlayAnimMontage(AttackAnimation);

	FTimerDelegate TimerDelegate = FTimerDelegate::CreateUObject( this, &ASCharacter::PrimaryAttack_TimeElapsed, DashProjectileClass);
	GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAttack, TimerDelegate,  0.2f, false);
	// GetWorldTimerManager().ClearTimer(TimerHandle_PrimaryAttack);
}

void ASCharacter::SpecialAttack()
{
	PlayAnimMontage(AttackAnimation);

	FTimerDelegate TimerDelegate = FTimerDelegate::CreateUObject( this, &ASCharacter::PrimaryAttack_TimeElapsed, BlackHoleProjectileClass);
	GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAttack, TimerDelegate,  0.2f, false);
	// GetWorldTimerManager().ClearTimer(TimerHandle_PrimaryAttack);
}

void ASCharacter::PrimaryInteract()
{
	if (InteractionComp)
	{
		InteractionComp->PrimaryInteract();
	}
}

void ASCharacter::OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta)
{
	if (NewHealth <= 0.0f && Delta < 0.0f)
	{
		DisableInput(Cast<APlayerController>(GetController()));
	}

	if (Delta < 0.0f)
	{
		GetMesh()->SetScalarParameterValueOnMaterials("HitFlashTime", GetWorld()->TimeSeconds);
	}
}

void ASCharacter::HealSelf(float Amount)
{
	AttributeComp->ApplyHealthChange(this, Amount);
}
