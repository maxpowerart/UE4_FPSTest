#include "Character/FPSCharacterBase.h"
#include "Character/FPSAbilitySystemComponent.h"

AFPSCharacterBase::AFPSCharacterBase()
{
	ASC = CreateDefaultSubobject<UFPSAbilitySystemComponent>("ASC");
}

UAbilitySystemComponent* AFPSCharacterBase::GetAbilitySystemComponent() const
{
	return ASC;
}

void AFPSCharacterBase::OnKilled()
{
	Destroy();
}
