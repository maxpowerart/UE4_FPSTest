#include "Weapon/Abilities/FPSAbility_WeaponReload.h"

#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Equipment/FPSEquipmentComponent.h"
#include "Inventory/FPSInventoryComponent.h"

UFPSAbility_WeaponReload::UFPSAbility_WeaponReload()
{
	AbilityInputID = EFPSAbilityInputID::Reload;
	AbilityTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.Weapon.Reload")));

	/**Can't fire during reload*/
	CancelAbilitiesWithTag.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.Weapon.Fire")));
	
	BlockAbilitiesWithTag.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.Weapon.Fire")));
	BlockAbilitiesWithTag.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.Weapon.Aim")));
}

bool UFPSAbility_WeaponReload::CanActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                                  const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags,
                                                  const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	if(!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags)) return false;
	if(!CheckCost(Handle, ActorInfo, nullptr)) return false;
	return true;
}

void UFPSAbility_WeaponReload::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	UFPSEquipmentComponent* EquipmentComponent = Cast<UFPSEquipmentComponent>(ActorInfo->AvatarActor->GetComponentByClass(UFPSEquipmentComponent::StaticClass()));

	/**Play reload anim*/
	UAnimMontage* TargetMontage = EquipmentComponent->GetWeaponList()[EquipmentComponent->GetWeaponIndex()].WeaponDA->ReloadMontage;
	if(!IsValid(TargetMontage))
	{
		OnEventReceived(FGameplayEventData());
		OnMontageEnd();
		return;
	}
	
	/**Wait for event task*/
	WaitForEventTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, FGameplayTag::RequestGameplayTag("Event.Weapon.Reload"));
	WaitForEventTask->EventReceived.AddDynamic(this, &UFPSAbility_WeaponReload::OnEventReceived);
	WaitForEventTask->ReadyForActivation();
	
	/**Play montage*/
	MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, NAME_None, TargetMontage);
	MontageTask->OnCompleted.AddDynamic(this, &UFPSAbility_WeaponReload::OnMontageEnd);
	MontageTask->OnInterrupted.AddDynamic(this, &UFPSAbility_WeaponReload::OnMontageEnd);
	MontageTask->OnCancelled.AddDynamic(this, &UFPSAbility_WeaponReload::OnMontageEnd);
	MontageTask->ReadyForActivation();
}
bool UFPSAbility_WeaponReload::CheckCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	FGameplayTagContainer* OptionalRelevantTags) const
{
	const UFPSEquipmentComponent* EquipmentComponent = Cast<UFPSEquipmentComponent>(ActorInfo->AvatarActor->GetComponentByClass(UFPSEquipmentComponent::StaticClass()));
	const UFPSInventoryComponent* InventoryComponent = EquipmentComponent->GetLinkedInventory();
	
	return (EquipmentComponent->GetCurrentSlot().WeaponDA->MaxAmmo - EquipmentComponent->GetCurrentSlot().AmmoInClip) > 0 &&
		InventoryComponent->GetItemQuantity(EquipmentComponent->GetCurrentSlot().WeaponDA->AmmoClass) > 0;
}
void UFPSAbility_WeaponReload::ApplyCost(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const
{
	UFPSEquipmentComponent* EquipmentComponent = Cast<UFPSEquipmentComponent>(ActorInfo->AvatarActor->GetComponentByClass(UFPSEquipmentComponent::StaticClass()));
	UFPSInventoryComponent* InventoryComponent = EquipmentComponent->GetLinkedInventory();

	const int32 NecessaryQuantity = EquipmentComponent->GetCurrentSlot().WeaponDA->MaxAmmo - EquipmentComponent->GetCurrentSlot().AmmoInClip;
	EquipmentComponent->GetWeaponList()[EquipmentComponent->GetWeaponIndex()].AmmoInClip += InventoryComponent->RemoveFromInventoryByType(EquipmentComponent->GetCurrentSlot().WeaponDA->AmmoClass, NecessaryQuantity);
}

void UFPSAbility_WeaponReload::OnEventReceived(FGameplayEventData Payload)
{	
	ApplyCost(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo);
}
void UFPSAbility_WeaponReload::OnMontageEnd()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}
