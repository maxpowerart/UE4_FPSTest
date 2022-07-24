#include "Character/FPSAbilitySystemComponent.h"

#include "Character/Abilities/FPSAbilityBase.h"

void UFPSAbilitySystemComponent::BeginPlay()
{
	Super::BeginPlay();

	/**Init default abilities*/
	for(const TSubclassOf<UFPSAbilityBase>& AbilityToGrant : DefaultAbilities)
	{
		GiveAbility(FGameplayAbilitySpec(AbilityToGrant, 1, static_cast<int32>(AbilityToGrant->GetDefaultObject<UFPSAbilityBase>()->AbilityInputID), this));
	}
}

void UFPSAbilitySystemComponent::AbilityLocalInputPressed(int32 InputID)
{
	Super::AbilityLocalInputPressed(InputID);
}

bool UFPSAbilitySystemComponent::IsActiveAbilityWithTag(FGameplayTag InTag)
{
	ABILITYLIST_SCOPE_LOCK();
	for (FGameplayAbilitySpec& Spec : ActivatableAbilities.Items)
	{
		if (Spec.IsActive() && Spec.Ability->AbilityTags.HasTag(InTag))
		{
			return true;
		}
	}
	return false;
}

TArray<TSubclassOf<UGameplayAbility>> UFPSAbilitySystemComponent::GetActiveAbilitiesList()
{
	ABILITYLIST_SCOPE_LOCK();
	TArray<TSubclassOf<UGameplayAbility>> Result;
	for (FGameplayAbilitySpec& Spec : ActivatableAbilities.Items)
	{
		if (Spec.IsActive())
		{
			Result.Add(Spec.Ability->GetClass());
		}
	}

	return Result;
}
