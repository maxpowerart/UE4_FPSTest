#include "Weapon/FPSPickableWeapon.h"

#include "GameFramework/PlayerState.h"
#include "Inventory/FPSInventoryComponent.h"
#include "Player/FPSPlayerCharacter.h"
#include "Weapon/DataAssets/FPSWeaponDataAsset.h"

void AFPSPickableWeapon::OnPicked_Implementation(AActor* Picker)
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

	if(Inventory->GetItemQuantity(ItemToPick) > 0)
	{
		/**If inventory already had this weapon - give him ammo*/
		Inventory->AddToInventory(Cast<UFPSWeaponDataAsset>(ItemToPick)->AmmoClass, Cast<UFPSWeaponDataAsset>(ItemToPick)->MaxAmmo);
	}
	else
	{
		Inventory->AddToInventory(ItemToPick, Quantity);
	}
	Destroy();
}
