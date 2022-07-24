#include "FPSAbilitySystemGlobals.h"
#include "Character/Abilities/Effects/FPSGameplayEffectContext.h"

FGameplayEffectContext* UFPSAbilitySystemGlobals::AllocGameplayEffectContext() const
{
	return new FFPSGameplayEffectContext();
}
