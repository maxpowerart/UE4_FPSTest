#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "FPSAbilitySystemComponent.generated.h"

UCLASS()
class UFPSAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;
	virtual void AbilityLocalInputPressed(int32 InputID) override;

	UFUNCTION(BlueprintPure)
	bool IsActiveAbilityWithTag(FGameplayTag InTag);
	UFUNCTION(BlueprintPure)
	TArray<TSubclassOf<UGameplayAbility>> GetActiveAbilitiesList();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config")
	TArray<TSubclassOf<class UFPSAbilityBase>> DefaultAbilities;
};
