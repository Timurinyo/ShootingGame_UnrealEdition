// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Camera/CameraComponent.h"
#include "Tank.generated.h"

//#include "GameFramework/CameraComponent.h"

//#PausePoint 32:26 USTRUCT
//#TODO Reinstall Vassist and setup it correctly
//This struct covers all possible input schemas

// USTRUCT is not UObject so you can't directly BindAxis to struct
USTRUCT(BlueprintType)
struct FTankInput
{
	// #Knowledge GENERATED_BODY macro is needed in all classes to support the infrastructure required by the engine.
	GENERATED_BODY()

public:
	//Sanitized movement input
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tank Input")
	FVector2D MovementInput;

	void Sanitize();
	void MoveX(float AxisValue);
	void MoveY(float AxisValue);

private:
	//Raw data
	FVector2D RawMovementInput;


};

UCLASS()
class TANKS_API ATank : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ATank();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	void MoveX(float AxisValue);
	void MoveY(float AxisValue);

	bool ShouldReverse(float deltaYaw);
	void Rotate(float deltaTime, float deltaYaw, float movementAngleYaw, bool bReverse);
	void Move(float deltaTime, bool bReverse);

	// Helpfull debug tool - which way is the tank facing
	UPROPERTY(
		VisibleAnywhere,
		BlueprintReadOnly,
		Category = "Tank",
		meta = (AllowPrivateAccess = "true"))
	class UArrowComponent* TankDirection;

	// Sprite for the tank body
	UPROPERTY(
		VisibleAnywhere,
		BlueprintReadOnly,
		Category = "Tank",
		meta = (AllowPrivateAccess = "true"))
	// TArray<USceneComponent>()
	class UPaperSpriteComponent* TankSprite;

	// The actor used as a turret
	UPROPERTY(
		VisibleAnywhere, 
		BlueprintReadOnly, 
		Category = "Tank", 
		meta = (AllowPrivateAccess = "true"))
	UChildActorComponent* ChildTurret;

	// #Knowledge For camera to worl properly in 2D you need to go to project Settings->Engine->Rendering->Translucensy(Sort Along Axis), Z = -1.0
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tank", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* CameraComponent;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tank Input")
	FTankInput TankInput;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tank Speed", meta = (ClampMin = "0.0"))
	float YawSpeed;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tank Speed", meta = (ClampMin = "0.0"))
	float MoveSpeed; 
};
