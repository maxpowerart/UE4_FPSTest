#include "Weapon/Abilities/FPSAbility_WeaponSwitchMode.h"
#include "Equipment/FPSEquipmentComponent.h"

UFPSAbility_WeaponSwitchMode::UFPSAbility_WeaponSwitchMode()
{
	AbilityInputID = EFPSAbilityInputID::AlternateFire;
	AbilityTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.Weapon.SwitchMode")));
}

bool UFPSAbility_WeaponSwitchMode::CanActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                                      const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags,
                                                      const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	if(!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags)) return false;

	/**Have more than one mode*/
	const UFPSEquipmentComponent* EquipmentComponent = Cast<UFPSEquipmentComponent>(ActorInfo->AvatarActor->GetComponentByClass(UFPSEquipmentComponent::StaticClass()));
	return EquipmentComponent->GetCurrentSlot().WeaponDA->AvailableShootingModes.Num() > 1;
}

void UFPSAbility_WeaponSwitchMode::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	UFPSEquipmentComponent* EquipmentComponent = Cast<UFPSEquipmentComponent>(ActorInfo->AvatarActor->GetComponentByClass(UFPSEquipmentComponent::StaticClass()));
	TArray<TEnumAsByte<EFPSShootingMode>> AvailableModesTemp = EquipmentComponent->GetCurrentSlot().WeaponDA->AvailableShootingModes.Array();
	const int32 NewMode = EquipmentComponent->GetWeaponIndex() == INDEX_NONE ? 0 :
		(AvailableModesTemp.Find(EquipmentComponent->GetCurrentSlot().ShootingMode) + 1) % AvailableModesTemp.Num();
	EquipmentComponent->GetWeaponList()[EquipmentComponent->GetWeaponIndex()].ShootingMode = AvailableModesTemp[NewMode];
	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}
