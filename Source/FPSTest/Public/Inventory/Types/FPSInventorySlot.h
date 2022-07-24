#pragma once

#include "CoreMinimal.h"
#include "Inventory/DataAssets/FPSItemBase.h"
#include "FPSInventorySlot.generated.h"

USTRUCT(BlueprintType)
struct FFPSInventorySlot
{
	GENERATED_BODY()

	FFPSInventorySlot(){}
	FFPSInventorySlot(UFPSItemBase* InItem, int32 InQuantity = 1) : Item(InItem), Quantity(InQuantity) {}

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UFPSItemBase* Item = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Meta = (UIMin = 1.f, ClampMin = 1.f))
	int32 Quantity = 1;
};
