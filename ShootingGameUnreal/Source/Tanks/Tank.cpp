// Fill out your copyright notice in the Description page of Project Settings.

#include "Tank.h"
#include "Components/ArrowComponent.h"
#include "Components/InputComponent.h"
#include "PaperSpriteComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "TanksStatics.h"

void FTankInput::Sanitize()
{
	MovementInput = RawMovementInput.ClampAxes(-1.0f, 1.0f);
	MovementInput = MovementInput.GetSafeNormal();
	RawMovementInput.Set(0.0f, 0.0f);
}

void FTankInput::MoveX(float AxisValue)
{
	RawMovementInput.X += AxisValue;
}

void FTankInput::MoveY(float AxisValue)
{
	// #Bug
	RawMovementInput.Y += AxisValue;
}

// Sets default values
ATank::ATank()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	if (!RootComponent)
	{
		//#KNOWLEDGE USE TEXT for strings
		RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("TankBase"));
	}

	TankDirection = CreateDefaultSubobject<UArrowComponent>(TEXT("TankBaseDirection"));
	TankDirection->AttachTo(RootComponent);

	TankSprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("TankSprite"));
	TankSprite->AttachTo(TankDirection);

	USpringArmComponent* SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->TargetArmLength = 500.f;
	SpringArm->bEnableCameraLag = true;
	SpringArm->bEnableCameraRotationLag = false;
	SpringArm->bUsePawnControlRotation = false;
	SpringArm->CameraLagSpeed = 2.f;
	SpringArm->bDoCollisionTest = false;
	SpringArm->AttachTo(RootComponent);
	SpringArm->SetWorldRotation(FRotator(-90.f, 0.f, 0.f));
	
	//#Knowledge create camera and attach it to the end of the spring arm
	//#Knowledge For camera to work properly, the auto posses property in the camera should be set to player 0...
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComponent->bUsePawnControlRotation = false;
	CameraComponent->ProjectionMode = ECameraProjectionMode::Orthographic;
	CameraComponent->OrthoWidth = 1024.f;
	CameraComponent->AspectRatio = 3.f / 4.f;
	CameraComponent->AttachTo(SpringArm, USpringArmComponent::SocketName);
	//CameraComponent->AttachTo(TankDirection);
	CameraComponent->SetWorldRotation(FRotator(0.f, 0.f, 0.f));//(-90.f, 0.f, 0.f));



	ChildTurret = CreateDefaultSubobject<UChildActorComponent>(TEXT("ChildTurret"));
	ChildTurret->AttachTo(TankDirection);

}

// Called when the game starts or when spawned
void ATank::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATank::Tick(float deltaTime)
{
	Super::Tick(deltaTime);
	
	// To avoid double speed and other...
	TankInput.Sanitize();

	// #Knowledge UE_LOG allows to conveniently log info in unreal
	//UE_LOG(LogTemp, Warning, TEXT("MovementInput: (%f %f)"), TankInput.MovementInput.X, TankInput.MovementInput.Y)

	// Move the tank!
	{
		FVector2D desiredMovementDirection = FVector2D(TankInput.MovementInput.X, TankInput.MovementInput.Y);
		if (!desiredMovementDirection.IsNearlyZero())
		{
			UE_LOG(LogTemp, Warning, TEXT("desiredMovementDirection: (%f %f)"), desiredMovementDirection.X, desiredMovementDirection.Y)

			float movementAngleYawRad = FMath::Atan2(desiredMovementDirection.Y, desiredMovementDirection.X) + PI/2;

			float movementAngleYaw = FMath::RadiansToDegrees(movementAngleYawRad);
			UE_LOG(LogTemp, Warning, TEXT("movementAngleYaw: %f"), movementAngleYaw)

			float deltaYaw = UTanksStatics::FindDeltaAngleDegrees(
				TankDirection->GetComponentRotation().Yaw, movementAngleYaw);
			UE_LOG(LogTemp, Warning, TEXT("deltaYaw: %f"), deltaYaw)

			bool bReverse = ShouldReverse(deltaYaw);
			Rotate(deltaTime, deltaYaw, movementAngleYaw, bReverse);
			Move(deltaTime, bReverse);
		}
	}
}

bool ATank::ShouldReverse(float deltaYaw)
{
	bool bReverse = false;
	if (deltaYaw != 0.f)
	{
		if (deltaYaw < -90.f || deltaYaw > 90.f)
		{
			bReverse = true;
		}
	}
	return bReverse;
}

void ATank::Rotate(float deltaTime, float deltaYaw, float movementAngleYaw, bool bReverse)
{
	if (deltaYaw == 0.f)
	{
		return;
	}

	float adjustedDeltaYaw = deltaYaw;

	if (adjustedDeltaYaw < -90.f)
	{
		adjustedDeltaYaw += 180.f;
	}
	else if (adjustedDeltaYaw > 90.f)
	{
		adjustedDeltaYaw -= 180.f;
	}

	float maxYawThisFrame = YawSpeed * deltaTime;
	if (maxYawThisFrame >= FMath::Abs(adjustedDeltaYaw))
	{
		// Can reach the destination during one frame
		float facingAngleYaw = movementAngleYaw;
		if (bReverse)
		{
			facingAngleYaw += 180.f;
		}
		TankDirection->SetWorldRotation(FRotator(0.f, facingAngleYaw, 0.f));
	}
	else
	{
		// Can't reach the destination during one frame
		TankDirection->AddLocalRotation(
			FRotator(
				0.f,
				FMath::Sign(adjustedDeltaYaw) * maxYawThisFrame,
				0.f)
		);
	}
}

void ATank::Move(float deltaTime, bool bReverse)
{
	//FVector2D tankForwardVector2D
	FVector movementDirection = TankDirection->GetForwardVector() * (bReverse ? -1.f : 1.f);
	FVector Pos = GetActorLocation(); // Later use collision
	Pos.X += movementDirection.X * MoveSpeed * deltaTime;
	Pos.Y += movementDirection.Y * MoveSpeed * deltaTime;
	SetActorLocation(Pos);
}

// Called to bind functionality to input
void ATank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	InputComponent->BindAxis("MoveX", this, &ATank::MoveX);
	InputComponent->BindAxis("MoveY", this, &ATank::MoveY);
}

void ATank::MoveX(float AxisValue)
{
	TankInput.MoveX(AxisValue);
}

void ATank::MoveY(float AxisValue)
{
	TankInput.MoveY(AxisValue);
}
