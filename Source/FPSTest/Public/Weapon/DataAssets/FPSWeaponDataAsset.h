#pragma once

#include "CoreMinimal.h"
#include "PaperSprite.h"
#include "Character/Abilities/FPSAbilityBase.h"
#include "Inventory/DataAssets/FPSItemBase.h"
#include "Weapon/FPSWeaponBase.h"
#include "MatineeCameraShake.h"
#include "Weapon/Types/FPSShootingMode.h"
#include "Weapon/Types/FPSShootingType.h"
#include "FPSWeaponDataAsset.generated.h"

UCLASS()
class UFPSWeaponDataAsset : public UFPSItemBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|General")
	TSubclassOf<AFPSWeaponBase> WeaponClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|General")
	TArray<TSubclassOf<UFPSAbilityBase>> GrantedAbilities;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (UIMin = 0.f, ClampMin = 0.f), Category = "Weapon|General")
	float Damage = 10.f;

	/**Used as start point for bullet. Also used for GCue*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|General")
	FName MuzzleSocket;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|CharacterAnim")
	UAnimSequence* WeaponIdle;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|CharacterAnim")
	UAnimSequence* WeaponRun;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|CharacterAnim")
	UAnimSequence* WeaponJumpStart;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|CharacterAnim")
	UAnimSequence* WeaponJumpLoop;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|CharacterAnim")
	UAnimSequence* WeaponJumpEnd;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|CharacterAnim")
	UAnimSequence* WeaponFire;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|CharacterAnim")
	UAnimSequence* WeaponAimFire;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|CharacterAnim")
	UAnimSequence* WeaponAim;
	
	/**Equipment properties*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Equip")
	bool bAutoEquip = true;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Equip")
	FName WeaponAttachPoint;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Equip")
	FTransform WeaponEquipedRelativeTransform;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Equip")
	UAnimMontage* EquipMontage;

	/**Shooting type (hitscan/projectile)*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|ShootingType")
	TEnumAsByte<EFPSShootingType> ShootingType;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (EditCondition = "ShootingType == EFPSShootingType::Projectile", EditConditionHides), Category = "Weapon|ShootingType")
	TSubclassOf<AActor> ProjectileClass;
	
	/**Shooting mode (semi-auto, full-auto, burst)*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|ShootingMode")
	TSet<TEnumAsByte<EFPSShootingMode>> AvailableShootingModes;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (EditCondition = "AvailableShootingModes.Contains(EFPSShootingMode::Burst)", EditConditionHides, UIMin = 1, UIMax = 1), Category = "Weapon|ShootingMode")
	int32 ShotsPerBurst = 3;
	
	/**Bullets per-shot quantity (should consume all bullets or just one, like shotgun)*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Bullets")
	bool bRandomBulletQuantity;
	/**How many bullets per shot. If !bRandomBulletQuantity - takes max*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Bullets")
	FInt32Range BulletsAmount = FInt32Range(1);;

	/**Rapidity*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (UIMin = 0.f, ClampMin = 0.f), Category = "Weapon|Rapidity")
	float PerShotDelay;

	/**Spreading*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (UIMin = 0.f, ClampMin = 0.f), Category = "Weapon|Spreading")
	float WeaponSpread = 1.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (UIMin = 0.f, ClampMin = 0.f), Category = "Weapon|Spreading")
	float WeaponSpreadIncrement = 1.2;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (UIMin = 0.f, ClampMin = 0.f), Category = "Weapon|Spreading")
	float WeaponSpreadMax = 10.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (UIMin = 0.f, ClampMin = 0.f), Category = "Weapon|Spreading")
	float DecreaseSpreadTime = 0.5f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (UIMin = 0.f, ClampMin = 0.f), Category = "Weapon|Spreading")
	float DecreaseSpreadDelay = 0.5f;

	/**Aiming*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (UIMin = 0.f, ClampMin = 0.f), Category = "Weapon|Aiming")
	float AimSpreadingMultiplier = 0.5f;

	/**TODO Effects (in gameplay cue): camera shake, muzzle, sound*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Effects")
	TSubclassOf<UMatineeCameraShake> CameraShakeClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Effects")
	UParticleSystem* MuzzleEmitter;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Effects")
	UParticleSystem* TrailEmitter;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Effects")
	USoundBase* ShotSound;

	/**Ammo type*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Ammo")
	UAnimMontage* ReloadMontage;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Ammo")
	UFPSItemBase* AmmoClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Ammo")
	int32 MaxAmmo = 30;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|UI")
	UPaperSprite* WeaponImage;
};
