#pragma once

#include "CoreMinimal.h"
#include "FPSItemBase.generated.h"

UCLASS()
class UFPSItemBase : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Visual")
	FText DisplayName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Visual")
	FText Description;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Visual")
	UTexture2D* Icon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "InventoryRules")
	bool bStackable = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (EditCondition = bStackable, UIMin = 1, ClampMin = 1), Category = "InventoryRules")
	int32 MaxStackQuantity = 1;
};
