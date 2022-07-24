#pragma once

#include "CoreMinimal.h"
#include "Character/Abilities/FPSAbilityBase.h"
#include "FPSAbility_WeaponFire.generated.h"

UCLASS()
class UFPSAbility_WeaponFire : public UFPSAbilityBase
{
	GENERATED_BODY()
	
public:
	UFPSAbility_WeaponFire();
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const override;
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	/**Check ammo, consume ammo*/
	virtual bool CheckCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		FGameplayTagContainer* OptionalRelevantTags) const override;
	virtual void ApplyCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo) const override;
	
protected:
	UFUNCTION(BlueprintCallable, Category = "Shooting")
	virtual void MakeShot(int32 ShotNum);
	UFUNCTION()
	void HandleTargetData(const FGameplayAbilityTargetDataHandle& Data);
	UFUNCTION()
	void OnTargetDataMissing(const FGameplayAbilityTargetDataHandle& Data);
	
	UFUNCTION()
	void OnInputReleased(float TimeHeld);
	UFUNCTION()
	void OnBurstFinished(int32 ShotNum);

	UPROPERTY()
	class UFPSEquipmentComponent* Equipment;

private:
	float LastShotTime = -1.f;
};
