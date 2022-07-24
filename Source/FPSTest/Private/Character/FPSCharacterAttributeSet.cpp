#include "Character/FPSCharacterAttributeSet.h"

#include "GameplayEffectExtension.h"
#include "Player/FPSPlayerState.h"

void UFPSCharacterAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	/**Here we should recalculate and adjust attribute, if its max value was changed (for ex, we should recalc current health, if MaxHealth was changed)*/
}

void UFPSCharacterAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	if(Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(), 0.0f, GetMaxHealth()));

		if(GetHealth() <= 0.f)
		{
			AController* Controller = Cast<APawn>(GetOwningActor())->GetController();
			if(APlayerController* PC = Cast<APlayerController>(Controller))
			{
				/**Send player to respawn*/
				PC->GetPlayerState<AFPSPlayerState>()->Kill();
			}
			else
			{
				/**Just destroy pawn*/
				GetOwningActor()->Destroy();
			}
		}
	}
}
