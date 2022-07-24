#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemGlobals.h"
#include "FPSAbilitySystemGlobals.generated.h"

UCLASS()
class FPSTEST_API UFPSAbilitySystemGlobals : public UAbilitySystemGlobals
{
	GENERATED_BODY()
	
public:
	static UFPSAbilitySystemGlobals& GSGet()
	{
		return dynamic_cast<UFPSAbilitySystemGlobals&>(Get());
	}

	virtual FGameplayEffectContext* AllocGameplayEffectContext() const override;
};
