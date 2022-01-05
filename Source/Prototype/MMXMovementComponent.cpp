// Fill out your copyright notice in the Description page of Project Settings.


#include "MMXMovementComponent.h"

// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerMovementComponent.h"
#include "Components/BoxComponent.h"

UMMXMovementComponent::UMMXMovementComponent()
{
	CurrentVelocity = FVector(0.f);
	Acceleration = FVector(0.f);
	MaxSpeed = 500.f;

	GroundAcceleration = MaxSpeed * 2;

	Gravity = FVector(0.f, 0.f, -2000.f);

	JumpStartSpeed = 150;
	JumpStartAcceleration = 1000;

	JumpStopSpeedReduction = 100;
	JumpStopAccelerationReduction = 200;

	IsGrounded = true;
	HitWall = false;
}

void UMMXMovementComponent::HorizontalMove(float Value)
{
	CurrentVelocity.X = FMath::Clamp(Value, -1.f, 1.f) * MaxSpeed;
}

void UMMXMovementComponent::Jump()
{
	if (IsGrounded)
	{
		IsGrounded = false;
		CurrentVelocity.Z = JumpStartSpeed;
		Acceleration.Z = JumpStartAcceleration;

		GetWorld()->GetTimerManager().SetTimer(JumpTimerHandle, this, &UMMXMovementComponent::JumpTimerFunc, .5f);
	}
}

void UMMXMovementComponent::StopJump()
{
	CurrentVelocity.Z -= JumpStopSpeedReduction;
	Acceleration.Z = -JumpStopAccelerationReduction;

	GetWorld()->GetTimerManager().ClearTimer(JumpTimerHandle);
}

void UMMXMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	UBoxComponent* Collider = Cast<UBoxComponent>(GetOwner()->GetRootComponent());

	BoxExtentX = Collider->GetScaledBoxExtent().X;
	BoxExtentZ = Collider->GetScaledBoxExtent().Z;
}

void UMMXMovementComponent::TickComponent(float DeltaTime, ELevelTick Tick,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, Tick, ThisTickFunction);

	// Check to make sure everything is still valid and that we are allowed to move
	if (!PawnOwner || !UpdatedComponent || ShouldSkipUpdate(DeltaTime))
	{
		return;
	}

	if (!IsGrounded)
	{
		Acceleration = Acceleration + Gravity * DeltaTime;
	}

	CurrentVelocity = CurrentVelocity + Acceleration * DeltaTime;

	const FVector DesiredMovementThisFrame = CurrentVelocity * DeltaTime;

	FHitResult Hit;
	SafeMoveUpdatedComponent(DesiredMovementThisFrame, UpdatedComponent->GetComponentRotation(), true, Hit);

	if (Hit.IsValidBlockingHit())
	{
		const FVector Loc = GetActorLocation();

		UE_LOG(LogTemp, Warning, TEXT("%f %f %f"), 
			Hit.ImpactPoint.X - Loc.X, 
			Hit.ImpactPoint.Y - Loc.Y,
			Hit.ImpactPoint.Z - Loc.Z);

		// Hit bottom.
		if (FMath::Abs(Loc.Z - Hit.ImpactPoint.Z - BoxExtentZ) < 3.f)
		{
			IsGrounded = true;
			Acceleration.Z = 0;
			CurrentVelocity.Z = 0;
		}

		// Hit Wall
		if (FMath::Abs(Loc.X - Hit.ImpactPoint.X - BoxExtentX) < 3.f)
		{
			Acceleration.X = 0;
			CurrentVelocity.X = 0;
		}
	}
}

void UMMXMovementComponent::JumpTimerFunc()
{
	StopJump();
}
