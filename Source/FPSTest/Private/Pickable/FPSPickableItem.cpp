#include "Pickable/FPSPickableItem.h"

#include "Character/FPSCharacterBase.h"
#include "GameFramework/PlayerState.h"
#include "Inventory/FPSInventoryComponent.h"
#include "Player/FPSPlayerCharacter.h"

void AFPSPickableItem::OnPicked_Implementation(AActor* Picker)
{
	/**
	 *  Get inventory component
	 */
	UFPSInventoryComponent* Inventory = Cast<UFPSInventoryComponent>(Picker->GetComponentByClass(UFPSInventoryComponent::StaticClass()));

	/**If picker is a player - add pickable to inventory in player state*/
	if(!IsValid(Inventory) && IsValid(Cast<AFPSPlayerCharacter>(Picker)))
	{
		Inventory = Cast<UFPSInventoryComponent>(
					Cast<APlayerController>(
						Cast<AFPSPlayerCharacter>(Picker)->GetController())->GetPlayerState<APlayerState>()->
							GetComponentByClass(UFPSInventoryComponent::StaticClass()));
	}
	if(!IsValid(Inventory)) return;

	Inventory->AddToInventory(ItemToPick, Quantity);
	Destroy();
}
