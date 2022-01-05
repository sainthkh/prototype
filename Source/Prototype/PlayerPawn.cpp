// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerPawn.h"

#include "MMXMovementComponent.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"

// Sets default values
APlayerPawn::APlayerPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	Collider = CreateDefaultSubobject<UBoxComponent>(TEXT("Collider"));
	Collider->InitBoxExtent(FVector(44.f, 44.f, 88.f));
	Collider->SetCollisionProfileName(UCollisionProfile::Pawn_ProfileName);
	Collider->bDynamicObstacle = true;
	RootComponent = Collider;
	
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(RootComponent);
	Camera->SetRelativeLocation(FVector(0.f, 1000.f, 00.f));
	Camera->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));

	Movement = CreateDefaultSubobject<UMMXMovementComponent>(TEXT("Movement"));
	Movement->UpdatedComponent = RootComponent;

	AutoReceiveInput = EAutoReceiveInput::Player0;
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

	
}

// Called to bind functionality to input
void APlayerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("Horizontal"), this, &APlayerPawn::HorizontalMove);
	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &APlayerPawn::Jump);
	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Released, this, &APlayerPawn::StopJump);
}

UPawnMovementComponent* APlayerPawn::GetMovementComponent() const
{
	return Movement;
}

void APlayerPawn::HorizontalMove(float Value)
{
	Movement->HorizontalMove((Value));
}

void APlayerPawn::Jump()
{
	Movement->Jump();
}

void APlayerPawn::StopJump()
{
	Movement->StopJump();
}
