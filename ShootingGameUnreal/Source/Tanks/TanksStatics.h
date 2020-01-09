// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "TanksStatics.generated.h"

/**
 * 
 */
UCLASS()
class TANKS_API UTanksStatics : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	// Angle diff in range [-180, 180]
	UFUNCTION(BlueprintCallable, Category = "Math")
	static float FindDeltaAngleDegrees(float angle1, float angle2);

	// Find the angle between two 2d points
	UFUNCTION(BlueprintPure, Category = "Math|Rotator", meta = (Keywords = "rotation rotate"))
	static bool FindLookAtAngle2D(const FVector2D& start, const FVector2D& target, float& angle);
};
