#include "Equipment/FPSEquipmentComponent.h"

#include "Character/FPSAbilitySystemComponent.h"
#include "GameFramework/Character.h"
#include "Inventory/FPSInventoryComponent.h"
#include "Player/FPSPlayerCharacter.h"

void UFPSEquipmentComponent::OnComponentDestroyed(bool bDestroyingHierarchy)
{
	for(const FFPSWeaponSlot& Slot : WeaponList)
	{
		if(Slot.WeaponDA == nullptr) continue;

		/**Destroy spawned instance*/
		if(IsValid(Slot.WeaponInstance)) Slot.WeaponInstance->Destroy();

		/**Unload ammo to inventory*/
		LinkedInventory->AddToInventory(Slot.WeaponDA->AmmoClass, Slot.AmmoInClip);
	}
	
	Super::OnComponentDestroyed(bDestroyingHierarchy);
}

void UFPSEquipmentComponent::LinkInventory(UFPSInventoryComponent* InventoryToLink)
{
	LinkedInventory = InventoryToLink;
	LinkedInventory->OnItemAdded.AddDynamic(this, &UFPSEquipmentComponent::OnItemPicked);
	LinkedInventory->OnItemRemoved.AddDynamic(this, &UFPSEquipmentComponent::OnItemRemoved);

	/**Try to equip any possible weapon*/
	for(const FFPSInventorySlot& Slot : LinkedInventory->GetInventorySlots())
	{
		if(Slot.Item->IsA(UFPSWeaponDataAsset::StaticClass()))
		{
			AddNewWeapon(Cast<UFPSWeaponDataAsset>(Slot.Item),0);
		}
	}

	for(FFPSWeaponSlot& WeaponSlot : WeaponList)
	{
		WeaponSlot.AmmoInClip = LinkedInventory->RemoveFromInventoryByType(WeaponSlot.WeaponDA->AmmoClass, WeaponSlot.WeaponDA->MaxAmmo);
	}
}

void UFPSEquipmentComponent::AddNewWeapon(UFPSWeaponDataAsset* NewWeapon, int32 AmmoInClip)
{
	/**Add unique weapon*/
	if(!WeaponList.Contains(NewWeapon))
	{
		if(AmmoInClip < 0) AmmoInClip = NewWeapon->MaxAmmo;
		WeaponList.Add(FFPSWeaponSlot(NewWeapon, FMath::Clamp(AmmoInClip, 0, NewWeapon->MaxAmmo)));
		WeaponList.Last().ShootingMode = WeaponList.Last().WeaponDA->AvailableShootingModes.Array()[0];
		
		if(CurrentWeaponIndex == INDEX_NONE)
		{
			SetWeaponIndex(0);
		}
	}
}
void UFPSEquipmentComponent::RemoveWeapon(UFPSWeaponDataAsset* WeaponToRemove)
{
	if(WeaponList.Contains(WeaponToRemove))
	{
		for(int32 WeaponIndex = 0; WeaponIndex < WeaponList.Num(); WeaponIndex++)
		{
			/**If we should remove weapon in hand - change weapon index*/
			if(WeaponList[WeaponIndex].WeaponDA == WeaponToRemove)
			{
				WeaponList.RemoveAt(WeaponIndex);
				return;
			}
		}
	}
}
void UFPSEquipmentComponent::SetWeaponIndex(int32 NewIndex)
{
	/**Swap weapon and spawn*/
	if(CurrentWeaponIndex != INDEX_NONE)
	{
		WeaponList[CurrentWeaponIndex].WeaponInstance->Destroy();
		
		/**Remove prev abilities*/
		UFPSAbilitySystemComponent* ASC = Cast<UFPSAbilitySystemComponent>(GetOwner()->GetComponentByClass(UFPSAbilitySystemComponent::StaticClass()));
		for(TSubclassOf<UFPSAbilityBase> Ability : WeaponList[CurrentWeaponIndex].WeaponDA->GrantedAbilities)
		{
			if(const FGameplayAbilitySpec* AbilitySpec = ASC->FindAbilitySpecFromClass(Ability))
			{
				ASC->SetRemoveAbilityOnEnd(AbilitySpec->Handle);
			}
		}
	}

	CurrentWeaponIndex = NewIndex;
	
	/**Spawn*/
	WeaponList[CurrentWeaponIndex].WeaponInstance = GetWorld()->SpawnActor<AFPSWeaponBase>(WeaponList[NewIndex].WeaponDA->WeaponClass);
	WeaponList[CurrentWeaponIndex].WeaponInstance->AttachToComponent(Cast<AFPSPlayerCharacter>(GetOwner())->GetFPSMesh(),
		FAttachmentTransformRules::SnapToTargetIncludingScale, WeaponList[NewIndex].WeaponDA->WeaponAttachPoint);
	WeaponList[CurrentWeaponIndex].WeaponInstance->SetActorRelativeTransform(WeaponList[CurrentWeaponIndex].WeaponDA->WeaponEquipedRelativeTransform);
	//WeaponList[CurrentWeaponIndex].WeaponInstance->SetOwner(GetOwner());

	/**Grant abilities*/
	UFPSAbilitySystemComponent* ASC = Cast<UFPSAbilitySystemComponent>(GetOwner()->GetComponentByClass(UFPSAbilitySystemComponent::StaticClass()));
	for(TSubclassOf<UFPSAbilityBase> Ability : WeaponList[CurrentWeaponIndex].WeaponDA->GrantedAbilities)
	{
		ASC->GiveAbility(FGameplayAbilitySpec(Ability, 1, static_cast<int32>(Ability->GetDefaultObject<UFPSAbilityBase>()->AbilityInputID),
			this));
	}
}

UFPSInventoryComponent* UFPSEquipmentComponent::GetLinkedInventory() const
{
	return LinkedInventory;
}

void UFPSEquipmentComponent::OnItemPicked(UFPSItemBase* Picked)
{
	/**If picked new weapon - add to list*/
	if(Cast<UFPSWeaponDataAsset>(Picked))
	{
		if(!WeaponList.Contains(Picked))
		{
			AddNewWeapon(Cast<UFPSWeaponDataAsset>(Picked));
		}
	}
}
void UFPSEquipmentComponent::OnItemRemoved(UFPSItemBase* Removed)
{
	/**If removed weapon - remove from list*/
	if(Cast<UFPSWeaponDataAsset>(Removed) && !WeaponList.Contains(Removed))
	{
		RemoveWeapon(Cast<UFPSWeaponDataAsset>(Removed));
	}
}
