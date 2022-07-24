#include "Inventory/FPSInventoryComponent.h"

void UFPSInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
	for(const TTuple<UFPSItemBase*, int>& Item : DefaultItems)
	{
		AddToInventory(Item.Key, Item.Value);
	}
}

void UFPSInventoryComponent::AddToInventory(UFPSItemBase* ItemToAdd, int32 Quantity)
{
	if(!IsValid(ItemToAdd) || Quantity < 1) return;

	/**If item is stackable - try to fill existing stacks*/
	int32 RemainingQuantity = Quantity;
	if(ItemToAdd->bStackable)
	{
		for(int32 SlotIndex = 0; SlotIndex < InventorySlots.Num(); SlotIndex++)
		{
			if(InventorySlots[SlotIndex].Item == ItemToAdd && InventorySlots[SlotIndex].Quantity < ItemToAdd->MaxStackQuantity)
			{
				const int32 AddedValue = FMath::Clamp(RemainingQuantity, 1, InventorySlots[SlotIndex].Quantity);
				RemainingQuantity -= AddedValue;
				InventorySlots[SlotIndex].Quantity += AddedValue;

				if(RemainingQuantity == 0)
				{
					OnItemAdded.Broadcast(ItemToAdd);
					return;
				}
			}
		}
	}

	/**Other way just add in inventory*/
	InventorySlots.Add(FFPSInventorySlot(ItemToAdd, RemainingQuantity));
	OnItemAdded.Broadcast(ItemToAdd);
}

int32 UFPSInventoryComponent::RemoveFromInventoryByType(UFPSItemBase* ItemToRemove, int32 Quantity)
{
	int32 RemainingQuantity = Quantity;
	for(int32 SlotIndex = 0; SlotIndex < InventorySlots.Num(); )
	{
		if(InventorySlots[SlotIndex].Item == ItemToRemove)
		{
			/**
			 * Note: if slot was removed, we shouldn't increment index
			 */
			const int32 RemovedValue = FMath::Clamp(RemainingQuantity, 1, InventorySlots[SlotIndex].Quantity);
			RemainingQuantity -= RemovedValue;
			InventorySlots[SlotIndex].Quantity -= RemovedValue;

			/**Remove slot, if it have no more quantity*/
			if(InventorySlots[SlotIndex].Quantity == 0) InventorySlots.RemoveAt(SlotIndex);
			
			/**If we removed enough - return*/
			if(RemainingQuantity == 0)
			{
				OnItemRemoved.Broadcast(ItemToRemove);
				return Quantity;
			}
		}
		else SlotIndex += 1;
	}
	if(RemainingQuantity != Quantity) OnItemRemoved.Broadcast(ItemToRemove);
	return Quantity - RemainingQuantity;
}

int32 UFPSInventoryComponent::RemoveFromInventoryByIndex(int32 Index, int32 Quantity)
{
	if(!InventorySlots.IsValidIndex(Index)) return 0;
	UFPSItemBase* TargetItem = InventorySlots[Index].Item;
	const int32 RemovedQuantity = FMath::Clamp(Quantity, 1, InventorySlots[Index].Quantity);
	InventorySlots[Index].Quantity -= RemovedQuantity;
	if(InventorySlots[Index].Quantity == 0) InventorySlots.RemoveAt(Index);
	OnItemRemoved.Broadcast(TargetItem);
	return RemovedQuantity;
}

int32 UFPSInventoryComponent::GetItemQuantity(UFPSItemBase* Item) const
{
	if(!IsValid(Item)) return 0;

	int32 ResultQuantity = 0;
	for(int32 SlotIndex = 0; SlotIndex < InventorySlots.Num(); SlotIndex++)
	{
		if(InventorySlots[SlotIndex].Item == Item) ResultQuantity += InventorySlots[SlotIndex].Quantity;
	}
	return ResultQuantity;
}
