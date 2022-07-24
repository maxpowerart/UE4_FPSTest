#pragma once

#include "CoreMinimal.h"
#include "Character/Abilities/FPSAbilityBase.h"
#include "FPSAbility_WeaponSwitchMode.generated.h"

UCLASS()
class UFPSAbility_WeaponSwitchMode : public UFPSAbilityBase
{
	GENERATED_BODY()
	
public:
	UFPSAbility_WeaponSwitchMode();
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const override;
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
};
