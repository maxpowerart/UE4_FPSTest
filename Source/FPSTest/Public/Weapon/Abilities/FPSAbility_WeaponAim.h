#pragma once

#include "CoreMinimal.h"
#include "Character/Abilities/FPSAbilityBase.h"
#include "FPSAbility_WeaponAim.generated.h"

UCLASS()
class UFPSAbility_WeaponAim : public UFPSAbilityBase
{
	GENERATED_BODY()
	
public:
	UFPSAbility_WeaponAim();
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;
};
