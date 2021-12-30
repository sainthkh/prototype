// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerPawn.h"
#include "Components/StaticMeshComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"

// Sets default values
APlayerPawn::APlayerPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(RootComponent);
	Camera->SetRelativeLocation(FVector(0.f, 1000.f, 00.f));
	Camera->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));

	AutoReceiveInput = EAutoReceiveInput::Player0;

	CurrentVelocity = FVector(0.f);
	Acceleration = FVector(0.f);
	Gravity = FVector(0.f, 0.f, -1000.f);
	MaxSpeed = 500.f;

	JumpStartSpeed = 150;
	JumpStartAcceleration = 1000;

	JumpStopSpeedReduction = 100;
	JumpStopAccelerationReduction = 200;

	IsGrounded = true;
}

// Called when the game starts or when spawned
void APlayerPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APlayerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!IsGrounded)
	{
		Acceleration = Acceleration + Gravity * DeltaTime;
	}

	CurrentVelocity = CurrentVelocity + Acceleration * DeltaTime;
	
	const FVector NewLocation = GetActorLocation() + (CurrentVelocity * DeltaTime);
	SetActorLocation(NewLocation);
}

// Called to bind functionality to input
void APlayerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("Horizontal"), this, &APlayerPawn::HorizontalMove);
	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &APlayerPawn::Jump);
	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Released, this, &APlayerPawn::StopJump);
}

void APlayerPawn::HorizontalMove(float Value)
{
	CurrentVelocity.X = FMath::Clamp(Value, -1.f, 1.f) * MaxSpeed;
}

void APlayerPawn::Jump()
{
	if (IsGrounded)
	{
		IsGrounded = false;
		CurrentVelocity.Z = JumpStartSpeed;
		Acceleration.Z = JumpStartAcceleration;

		const FVector NewLocation = GetActorLocation() + FVector(0, 0, 4.f);
		SetActorLocation(NewLocation);

		GetWorldTimerManager().SetTimer(JumpTimerHandle, this, &APlayerPawn::JumpTimerFunc, .5f);
	}
}

void APlayerPawn::StopJump()
{
	CurrentVelocity.Z -= JumpStopSpeedReduction;
	Acceleration.Z = -JumpStopAccelerationReduction;

	GetWorldTimerManager().ClearTimer(JumpTimerHandle);
}

void APlayerPawn::JumpTimerFunc()
{
	StopJump();
}
