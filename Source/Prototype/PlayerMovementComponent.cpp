// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerMovementComponent.h"
#include "Components/CapsuleComponent.h"

UPlayerMovementComponent::UPlayerMovementComponent()
{
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

void UPlayerMovementComponent::HorizontalMove(float Value)
{
	CurrentVelocity.X = FMath::Clamp(Value, -1.f, 1.f) * MaxSpeed;
}

void UPlayerMovementComponent::Jump()
{
	if (IsGrounded)
	{
		IsGrounded = false;
		CurrentVelocity.Z = JumpStartSpeed;
		Acceleration.Z = JumpStartAcceleration;

		GetWorld()->GetTimerManager().SetTimer(JumpTimerHandle, this, &UPlayerMovementComponent::JumpTimerFunc, .5f);
	}
}

void UPlayerMovementComponent::StopJump()
{
	CurrentVelocity.Z -= JumpStopSpeedReduction;
	Acceleration.Z = -JumpStopAccelerationReduction;

	GetWorld()->GetTimerManager().ClearTimer(JumpTimerHandle);
}

void UPlayerMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	CapsuleHalfHeight = Cast<UCapsuleComponent>(GetOwner()->GetRootComponent())->GetScaledCapsuleHalfHeight();
}

void UPlayerMovementComponent::TickComponent(float DeltaTime, ELevelTick Tick,
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

		// Hit bottom.
		if(FMath::Abs(Loc.Z - Hit.ImpactPoint.Z - CapsuleHalfHeight) < 1.f)
		{
			IsGrounded = true;
			Acceleration.Z = 0;
			CurrentVelocity.Z = 0;
		}
	}
}

void UPlayerMovementComponent::JumpTimerFunc()
{
	StopJump();
}
