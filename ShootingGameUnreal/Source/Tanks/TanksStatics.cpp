// Fill out your copyright notice in the Description page of Project Settings.


#include "TanksStatics.h"

float UTanksStatics::FindDeltaAngleDegrees(float angle1, float angle2)
{
	float delta = angle2 - angle1;

	if (delta > 180.0f)
	{
		delta = delta - 360.0f;
	}
	else if (delta < -180.0f)
	{
		delta = delta + 360.0f;
	}

	return delta;
}

bool UTanksStatics::FindLookAtAngle2D(const FVector2D& start, const FVector2D& target, float& angle)
{
	FVector2D normal = (target - start).GetSafeNormal();
	if (!normal.IsNearlyZero())
	{
		angle = FMath::RadiansToDegrees(FMath::Atan2(normal.Y, normal.X));
		return true;
	}
	return false;
}
