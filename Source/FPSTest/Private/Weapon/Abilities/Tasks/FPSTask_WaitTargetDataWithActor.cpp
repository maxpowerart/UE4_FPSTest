#include "Weapon/Abilities/Tasks/FPSTask_WaitTargetDataWithActor.h"

void UFPSTask_WaitTargetDataWithActor::Activate()
{
	if (IsPendingKill())
	{
		return;
	}

	if (Ability && TargetActor)
	{
		/**Binds to target data callbacks*/
		TargetActor->TargetDataReadyDelegate.AddUObject(this, &UFPSTask_WaitTargetDataWithActor::OnTargetDataReadyCallback);
		TargetActor->CanceledDelegate.AddUObject(this, &UFPSTask_WaitTargetDataWithActor::OnTargetDataCancelledCallback);

		/**Start targeting*/
		TargetActor->StartTargeting(Ability);

		// If instant confirm, then stop targeting immediately.
		if (ConfirmationType == EGameplayTargetingConfirmation::Instant)
		{
			TargetActor->ConfirmTargeting();
		}
		else if (ConfirmationType == EGameplayTargetingConfirmation::UserConfirmed)
		{
			// Bind to the Cancel/Confirm Delegates (called from local confirm or from repped confirm)
			TargetActor->BindToConfirmCancelInputs();
		}
	}
	else
	{
		EndTask();
	}
}

void UFPSTask_WaitTargetDataWithActor::ExternalConfirm(bool bEndTask)
{
	Super::ExternalConfirm(bEndTask);
}

void UFPSTask_WaitTargetDataWithActor::ExternalCancel()
{
	Super::ExternalCancel();
}

UFPSTask_WaitTargetDataWithActor* UFPSTask_WaitTargetDataWithActor::WaitTargetDataWithActor(
	UGameplayAbility* OwningAbility, FName TaskInstanceName,
	TEnumAsByte<EGameplayTargetingConfirmation::Type> ConfirmationType, AGameplayAbilityTargetActor* InTargetActor)
{
	UFPSTask_WaitTargetDataWithActor* MyObj = NewAbilityTask<UFPSTask_WaitTargetDataWithActor>(OwningAbility, TaskInstanceName);		//Register for task list here, providing a given FName as a key
	MyObj->TargetActor = InTargetActor;
	MyObj->ConfirmationType = ConfirmationType;
	return MyObj;
}

void UFPSTask_WaitTargetDataWithActor::OnTargetDataReadyCallback(const FGameplayAbilityTargetDataHandle& Data)
{
	if(ShouldBroadcastAbilityTaskDelegates()) ValidData.Broadcast(Data);

	if(ConfirmationType != EGameplayTargetingConfirmation::CustomMulti) EndTask();
}

void UFPSTask_WaitTargetDataWithActor::OnTargetDataCancelledCallback(const FGameplayAbilityTargetDataHandle& Data)
{
	Cancelled.Broadcast(Data);
	EndTask();
}
