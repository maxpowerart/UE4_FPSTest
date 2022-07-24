// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EFPSAbilityInputID : uint8
{
	// 0 None
	None				UMETA(DisplayName = "None"),
	// 1 Jump
	Jump				UMETA(DisplayName = "Jump"),
	// 2 PrimaryFire
	PrimaryFire			UMETA(DisplayName = "Primary Fire"),
	// 3 SecondaryFire
	SecondaryFire		UMETA(DisplayName = "Secondary Fire"),
	// 4 Alternate Fire
	AlternateFire		UMETA(DisplayName = "Alternate Fire"),
	// 5 Reload
	Reload				UMETA(DisplayName = "Reload"),
	// 6 NextWeapon
	NextWeapon			UMETA(DisplayName = "Next Weapon"), 
	// 7 PrevWeapon
	PrevWeapon			UMETA(DisplayName = "Previous Weapon"),
	// 8 Confirm
	Confirm			UMETA(DisplayName = "Confirm"),
	// 9 Cancel
	Cancel			UMETA(DisplayName = "Cancel")
};