#pragma once

#include "CoreMinimal.h"
#include "FPSCalc_Damage.h"
#include "GameplayEffect.h"
#include "Character/FPSCharacterAttributeSet.h"
#include "FPSEffect_Damage.generated.h"

UCLASS()
class UFPSEffect_Damage : public UGameplayEffect
{
	GENERATED_BODY()
	
public:
	UFPSEffect_Damage()
	{
		FGameplayEffectExecutionDefinition Execution;
		Execution.CalculationClass = UFPSCalc_Damage::StaticClass();
		Executions.Add(Execution);
	}
};
