#pragma once

#include "CoreMinimal.h"
#include "Types/FPSInventorySlot.h"
#include "FPSInventoryComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FFPSOnItemQuantityChanged, UFPSItemBase*, Item);
UCLASS()
class UFPSInventoryComponent : public UActorComponent
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;
	
	UFUNCTION(BlueprintCallable, Category = "InventoryManagement")
	void AddToInventory(class UFPSItemBase* ItemToAdd, int32 Quantity = 1);
	UFUNCTION(BlueprintCallable, Category = "InventoryManagement")
	int32 RemoveFromInventoryByType(class UFPSItemBase* ItemToRemove, int32 Quantity = 1);
	UFUNCTION(BlueprintCallable, Category = "InventoryManagement")
	int32 RemoveFromInventoryByIndex(int32 Index, int32 Quantity = 1);

	UFUNCTION(BlueprintPure, Category = "Getters")
	const TArray<FFPSInventorySlot>& GetInventorySlots() const { return InventorySlots; }
	UFUNCTION(BlueprintPure, Category = "Getters")
	int32 GetItemQuantity(class UFPSItemBase* Item) const;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config")
	TMap<UFPSItemBase*, int32> DefaultItems;

	UPROPERTY(BlueprintAssignable)
	FFPSOnItemQuantityChanged OnItemAdded;

	UPROPERTY(BlueprintAssignable)
	FFPSOnItemQuantityChanged OnItemRemoved;
	
protected:
	UPROPERTY(VisibleAnywhere, Category = "Runtime")
	TArray<FFPSInventorySlot> InventorySlots;
};
