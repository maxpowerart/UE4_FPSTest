#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Character/Abilities/FPSAbilityBase.h"
#include "FPSAbility_WeaponReload.generated.h"

UCLASS()
class UFPSAbility_WeaponReload : public UFPSAbilityBase
{
	GENERATED_BODY()
	
public:
	UFPSAbility_WeaponReload();
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const override;
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual bool CheckCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, FGameplayTagContainer* OptionalRelevantTags) const override;
	virtual void ApplyCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const override;

protected:
	UPROPERTY()
	UAbilityTask_PlayMontageAndWait* MontageTask;
	UPROPERTY()
	UAbilityTask_WaitGameplayEvent* WaitForEventTask;

private:
	UFUNCTION()
	void OnEventReceived(FGameplayEventData Payload);
	UFUNCTION()
	void OnMontageEnd();
};
