#pragma once

#include "CoreMinimal.h"
#include "Weapon/FPSWeaponBase.h"
#include "Weapon/DataAssets/FPSWeaponDataAsset.h"
#include "FPSWeaponSlot.generated.h"

/**Holds weapon asset and any runtime properties, which should be saved during swap to other weapon*/
USTRUCT(BlueprintType)
struct FFPSWeaponSlot
{
	GENERATED_BODY()

	FFPSWeaponSlot() {}
	FFPSWeaponSlot(UFPSWeaponDataAsset* InWeapon) : WeaponDA(InWeapon) {}
	FFPSWeaponSlot(UFPSWeaponDataAsset* InWeapon, int32 InAmmoInClip) : WeaponDA(InWeapon), AmmoInClip(InAmmoInClip) {}

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UFPSWeaponDataAsset* WeaponDA = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	AFPSWeaponBase* WeaponInstance = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TEnumAsByte<EFPSShootingMode> ShootingMode = EFPSShootingMode::Single;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 AmmoInClip = 0;

	FORCEINLINE bool operator ==(const UFPSItemBase* Right) const
	{
		return WeaponDA == Right;
	}
	FORCEINLINE bool operator !=(const UFPSItemBase* Right) const
	{
		return WeaponDA != Right;
	}
};
