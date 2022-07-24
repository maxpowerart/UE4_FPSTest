#include "Weapon/Abilities/FPSAbility_WeaponFire.h"

#include "AbilitySystemComponent.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "Abilities/Tasks/AbilityTask_Repeat.h"
#include "Abilities/Tasks/AbilityTask_WaitInputRelease.h"
#include "Equipment/FPSEquipmentComponent.h"
#include "Character/Abilities/Effects/FPSEffect_Damage.h"
#include "Character/Abilities/Effects/FPSGameplayEffectContext.h"
#include "Weapon/Abilities/Targeting/FPSTarget_Trace.h"
#include "Weapon/Abilities/Tasks/FPSTask_WaitTargetDataWithActor.h"

UFPSAbility_WeaponFire::UFPSAbility_WeaponFire()
{
	AbilityInputID = EFPSAbilityInputID::PrimaryFire;
	AbilityTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.Weapon.Fire")));
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

bool UFPSAbility_WeaponFire::CanActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                                const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags,
                                                const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	if(!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags)) return false;

	/**Have ammo*/
	if(!CheckCost(Handle, ActorInfo, nullptr)) return false;

	/**Have no delay between shots*/
	const UFPSEquipmentComponent* TempEquipment = Cast<UFPSEquipmentComponent>(ActorInfo->AvatarActor->GetComponentByClass(UFPSEquipmentComponent::StaticClass()));
	if(!IsValid(TempEquipment) || FMath::Abs(ActorInfo->AvatarActor->GetWorld()->GetTimeSeconds() - LastShotTime) < TempEquipment->GetCurrentSlot().WeaponDA->PerShotDelay) return false;

	return true;
}

void UFPSAbility_WeaponFire::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Equipment = Cast<UFPSEquipmentComponent>(ActorInfo->AvatarActor->GetComponentByClass(UFPSEquipmentComponent::StaticClass()));

	const UFPSWeaponDataAsset* WeaponDA = Equipment->GetCurrentSlot().WeaponDA;
	AFPSWeaponBase* WeaponInstance = Equipment->GetCurrentSlot().WeaponInstance;

	if(Equipment->GetCurrentSlot().ShootingMode == EFPSShootingMode::Single)
	{
		MakeShot(1);
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
	}
	else if(Equipment->GetCurrentSlot().ShootingMode == EFPSShootingMode::Burst)
	{
		UAbilityTask_Repeat* RepeatTask = UAbilityTask_Repeat::RepeatAction(this, WeaponDA->PerShotDelay, WeaponDA->ShotsPerBurst);
		RepeatTask->OnPerformAction.AddDynamic(this, &UFPSAbility_WeaponFire::MakeShot);
		RepeatTask->OnFinished.AddDynamic(this, &UFPSAbility_WeaponFire::OnBurstFinished);
		RepeatTask->ReadyForActivation();
	}
	else if(Equipment->GetCurrentSlot().ShootingMode == EFPSShootingMode::FullAuto)
	{
		UAbilityTask_WaitInputRelease* InputTask = UAbilityTask_WaitInputRelease::WaitInputRelease(this, true);
		InputTask->OnRelease.AddDynamic(this, &UFPSAbility_WeaponFire::OnInputReleased);
		InputTask->ReadyForActivation();

		FTimerHandle ShootHandle;
		const FTimerDelegate Delegate = FTimerDelegate::CreateUObject(this, &UFPSAbility_WeaponFire::MakeShot, 0);
		GetWorld()->GetTimerManager().SetTimer(ShootHandle, Delegate,  WeaponDA->PerShotDelay, true, 0);
	}
}

bool UFPSAbility_WeaponFire::CheckCost(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, FGameplayTagContainer* OptionalRelevantTags) const
{
	if(!Super::CheckCost(Handle, ActorInfo, OptionalRelevantTags)) return false;
	
	/**Check ammo in clip*/
	const UFPSEquipmentComponent* TempEquipment = Cast<UFPSEquipmentComponent>(ActorInfo->AvatarActor->GetComponentByClass(UFPSEquipmentComponent::StaticClass()));
	if(!IsValid(TempEquipment) || TempEquipment->GetCurrentSlot().AmmoInClip == 0)
	{
		/**Request reload*/
		TArray<FGameplayTag> TagContainer;
		TagContainer.Add(FGameplayTag::RequestGameplayTag(FName("Ability.Weapon.Reload")));
		ActorInfo->AbilitySystemComponent->TryActivateAbilitiesByTag(FGameplayTagContainer::CreateFromArray(TagContainer));
		return false;
	}
	return true;
}
void UFPSAbility_WeaponFire::ApplyCost(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const
{
	/**Consume ammo in clip*/
	Equipment->GetWeaponList()[Equipment->GetWeaponIndex()].AmmoInClip -= 1;
}

void UFPSAbility_WeaponFire::OnInputReleased(float TimeHeld)
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}
void UFPSAbility_WeaponFire::OnBurstFinished(int32 ShotNum)
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UFPSAbility_WeaponFire::MakeShot(int32 ShotNum)
{
	/**If have no ammo - end ability*/
	if(!CheckCost(CurrentSpecHandle, CurrentActorInfo, nullptr))
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
		return;
	}
	
	/**Targeting*/
	AFPSTarget_Trace* Target = Equipment->GetCurrentSlot().WeaponInstance->GetTargetActor();
	UFPSTask_WaitTargetDataWithActor* TargetTask = UFPSTask_WaitTargetDataWithActor::WaitTargetDataWithActor(this, NAME_None, EGameplayTargetingConfirmation::Instant, Target);
	TargetTask->ValidData.AddDynamic(this, &UFPSAbility_WeaponFire::HandleTargetData);
	TargetTask->Cancelled.AddDynamic(this, &UFPSAbility_WeaponFire::OnTargetDataMissing);
	TargetTask->ReadyForActivation();

	/**Remember last shot timing*/
	LastShotTime = CurrentActorInfo->AvatarActor->GetWorld()->GetTimeSeconds();
}

void UFPSAbility_WeaponFire::HandleTargetData(const FGameplayAbilityTargetDataHandle& Data)
{
	/**Consume bullet in clip*/
	ApplyCost(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo);
	
	/**Make cue params*/
	FGameplayEffectSpecHandle Handle = MakeOutgoingGameplayEffectSpec(UFPSEffect_Damage::StaticClass());
	Handle.Data.Get()->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Data.Damage")), Equipment->GetCurrentSlot().WeaponDA->Damage);
	ApplyGameplayEffectSpecToTarget(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, Handle, Data);

	FGameplayEffectContextHandle ContextHandle = Handle.Data.Get()->GetEffectContext();

	/**Add target data to context handle*/
	FFPSGameplayEffectContext* EffectContext = static_cast<FFPSGameplayEffectContext*>(ContextHandle.Get());
	if(EffectContext != nullptr)
	{
		EffectContext->AddTargetData(Data);
	}
	else check(0);
	
	CurrentActorInfo->AbilitySystemComponent->ExecuteGameplayCue(FGameplayTag::RequestGameplayTag(FName("GameplayCue.Weapon.Fire")), ContextHandle);
}

void UFPSAbility_WeaponFire::OnTargetDataMissing(const FGameplayAbilityTargetDataHandle& Data)
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}