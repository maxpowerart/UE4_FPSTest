#include "Character/Abilities/Effects//FPSCalc_Damage.h"

#include "Character/FPSCharacterAttributeSet.h"

// Declare the attributes to capture and define how we want to capture them from the Source and Target.
struct GSDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Health);

	GSDamageStatics()
	{
		// Capture the Target's Armor. Don't snapshot.
		DEFINE_ATTRIBUTE_CAPTUREDEF(UFPSCharacterAttributeSet, Health, Target, false);
	}
};

static const GSDamageStatics& DamageStatics()
{
	static GSDamageStatics DStatics;
	return DStatics;
}

UFPSCalc_Damage::UFPSCalc_Damage()
{
}

void UFPSCalc_Damage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
	
	float Damage = 0.0f;
	//ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().HealthDef, FAggregatorEvaluateParameters(), Damage);
	Damage -= FMath::Max<float>(Spec.GetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Data.Damage")), false, -1.0f), 0.0f);
	OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(DamageStatics().HealthProperty, EGameplayModOp::Additive, Damage));
}
