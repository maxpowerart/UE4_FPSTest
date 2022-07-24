#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbilityTargetActor.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "FPSTask_WaitTargetDataWithActor.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FWaitTargetDataWithActor, const FGameplayAbilityTargetDataHandle&, Data);

UCLASS()
class UFPSTask_WaitTargetDataWithActor : public UAbilityTask
{
	GENERATED_BODY()
	
public:
	/**Overrides*/
	virtual void Activate() override;
	virtual void ExternalConfirm(bool bEndTask) override;
	virtual void ExternalCancel() override;

	/**Static method for instancing task*/
	UFUNCTION(BlueprintCallable, meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "true", HideSpawnParms = "Instigator"), Category = "Ability|Tasks")
	static UFPSTask_WaitTargetDataWithActor* WaitTargetDataWithActor(
		UGameplayAbility* OwningAbility,
		FName TaskInstanceName,
		TEnumAsByte<EGameplayTargetingConfirmation::Type> ConfirmationType,
		AGameplayAbilityTargetActor* InTargetActor);

	/**Delegate-pins*/
	UPROPERTY(BlueprintAssignable)
	FWaitTargetDataWithActor ValidData;
	UPROPERTY(BlueprintAssignable)
	FWaitTargetDataWithActor Cancelled;

protected:
	/**Callbacks for target actor*/
	UFUNCTION()
	virtual void OnTargetDataReadyCallback(const FGameplayAbilityTargetDataHandle& Data);
	UFUNCTION()
	virtual void OnTargetDataCancelledCallback(const FGameplayAbilityTargetDataHandle& Data);
	
	UPROPERTY()
	AGameplayAbilityTargetActor* TargetActor;

	TEnumAsByte<EGameplayTargetingConfirmation::Type> ConfirmationType;
};
