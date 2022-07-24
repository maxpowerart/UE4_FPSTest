﻿#include "Equipment/Abilities/FPSAbility_NextWeapon.h"

#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Equipment/FPSEquipmentComponent.h"
#include "Equipment/Abilities/FPSAbility_PrevWeapon.h"

UFPSAbility_NextWeapon::UFPSAbility_NextWeapon()
{
	AbilityInputID = EFPSAbilityInputID::NextWeapon;
	AbilityTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.Weapon.IsChanging")));

	BlockAbilitiesWithTag.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.Weapon.Reload")));
	BlockAbilitiesWithTag.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.Weapon.Fire")));
	BlockAbilitiesWithTag.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.Weapon.SwitchMode")));
	BlockAbilitiesWithTag.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.Weapon.Aim")));
	
	CancelAbilitiesWithTag.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.Weapon.IsChanging")));
	CancelAbilitiesWithTag.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.Weapon.Fire")));
	CancelAbilitiesWithTag.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.Weapon.Reload")));
	CancelAbilitiesWithTag.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.Weapon.Aim")));
}

bool UFPSAbility_NextWeapon::CanActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                                const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags,
                                                const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	if(!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags)) return false;

	/**Get equipment component*/
	UFPSEquipmentComponent* EquipmentComponent = Cast<UFPSEquipmentComponent>(ActorInfo->AvatarActor->GetComponentByClass(UFPSEquipmentComponent::StaticClass()));
	if (!IsValid(EquipmentComponent) || EquipmentComponent->GetWeaponList().Num() < 2)
	{
		return false;
	}
	
	return true;
}

void UFPSAbility_NextWeapon::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	UFPSEquipmentComponent* EquipmentComponent = Cast<UFPSEquipmentComponent>(ActorInfo->AvatarActor->GetComponentByClass(UFPSEquipmentComponent::StaticClass()));
	const int32 NewWeaponIndex = EquipmentComponent->GetWeaponIndex() == INDEX_NONE ? 0 : (EquipmentComponent->GetWeaponIndex() + 1) % EquipmentComponent->GetWeaponList().Num();
	EquipmentComponent->SetWeaponIndex(NewWeaponIndex);

	/**Reset montage*/
	if(IsValid(ChangeMontage)) ChangeMontage->ExternalCancel();

	/**Play equip anim*/
	UAnimMontage* TargetMontage = EquipmentComponent->GetWeaponList()[EquipmentComponent->GetWeaponIndex()].WeaponDA->EquipMontage;
	if(!IsValid(TargetMontage)) EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
	ChangeMontage = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, NAME_None, TargetMontage);
	ChangeMontage->OnCompleted.AddDynamic(this, &UFPSAbility_NextWeapon::OnMontageEnd);
	ChangeMontage->OnInterrupted.AddDynamic(this, &UFPSAbility_NextWeapon::OnMontageEnd);
	ChangeMontage->OnCancelled.AddDynamic(this, &UFPSAbility_NextWeapon::OnMontageEnd);
	ChangeMontage->ReadyForActivation();
}

void UFPSAbility_NextWeapon::OnMontageEnd()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}