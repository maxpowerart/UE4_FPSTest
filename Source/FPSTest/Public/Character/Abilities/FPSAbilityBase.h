#pragma once

#include "CoreMinimal.h"
#include "FPSTest.h"
#include "Abilities/GameplayAbility.h"
#include "FPSAbilityBase.generated.h"

UCLASS()
class UFPSAbilityBase : public UGameplayAbility
{
	GENERATED_BODY()
	
public:
	// Abilities will automatically activate when the input is pressed
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability")
	EFPSAbilityInputID AbilityInputID = EFPSAbilityInputID::None;
};
