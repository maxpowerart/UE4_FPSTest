#include "Weapon/Abilities/FPSAbility_WeaponAim.h"

UFPSAbility_WeaponAim::UFPSAbility_WeaponAim()
{
	AbilityInputID = EFPSAbilityInputID::SecondaryFire;
	AbilityTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.Weapon.Aim")));
}

void UFPSAbility_WeaponAim::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	/**Begin transition*/

	/**Slow MS*/

	/**Change fov */
}

void UFPSAbility_WeaponAim::InputReleased(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	/**Begin reversed transition*/

	/**Restore fov */
	
	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}
