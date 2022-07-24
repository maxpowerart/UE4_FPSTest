#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "FPSCharacterBase.generated.h"

UCLASS()
class AFPSCharacterBase : public ACharacter,
						  public IAbilitySystemInterface
{
	GENERATED_BODY()
	
public:
	AFPSCharacterBase();

	/**IAbilitySystemInterface implementation*/
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	virtual void OnKilled();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	class UFPSAbilitySystemComponent* ASC;
};
