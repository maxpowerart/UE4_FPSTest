#pragma once

#include "CoreMinimal.h"
#include "Types/FPSWeaponSlot.h"
#include "FPSEquipmentComponent.generated.h"

UCLASS()
class UFPSEquipmentComponent : public UActorComponent
{
	GENERATED_BODY()
	
public:
	virtual void OnComponentDestroyed(bool bDestroyingHierarchy) override;
	
	UFUNCTION(BlueprintCallable, Category = "Initializing")
	void LinkInventory(class UFPSInventoryComponent* InventoryToLink);

	UFUNCTION(BlueprintCallable, Category = "WeaponManagement")
	void AddNewWeapon(class UFPSWeaponDataAsset* NewWeapon, int32 AmmoInClip = -1);
	UFUNCTION(BlueprintCallable, Category = "WeaponManagement")
	void RemoveWeapon(class UFPSWeaponDataAsset* WeaponToRemove);
	UFUNCTION(BlueprintCallable, Category = "WeaponManagement")
	void SetWeaponIndex(int32 NewIndex);

	UFUNCTION(BlueprintCallable, Category = "Getters")
	TArray<FFPSWeaponSlot>& GetWeaponList() { return WeaponList; }
	UFUNCTION(BlueprintCallable, Category = "Getters")
	int32 GetWeaponIndex() const { return CurrentWeaponIndex; }
	UFUNCTION(BlueprintCallable, Category = "Getters")
	FFPSWeaponSlot GetCurrentSlot() const { return WeaponList.IsValidIndex(CurrentWeaponIndex) ? WeaponList[CurrentWeaponIndex] : FFPSWeaponSlot(); }
	UFUNCTION(BlueprintCallable, Category = "Getters")
	UFPSInventoryComponent* GetLinkedInventory() const;
	
protected:
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Runtime")
	int32 CurrentWeaponIndex = INDEX_NONE;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Runtime")
	TArray<FFPSWeaponSlot> WeaponList;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Links")
	class UFPSInventoryComponent* LinkedInventory;
	
private:
	UFUNCTION()
	void OnItemPicked(UFPSItemBase* Picked);
	UFUNCTION()
	void OnItemRemoved(UFPSItemBase* Removed);
};
