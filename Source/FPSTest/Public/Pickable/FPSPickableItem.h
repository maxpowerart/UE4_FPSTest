#pragma once

#include "CoreMinimal.h"
#include "FPSPickableBase.h"
#include "FPSPickableItem.generated.h"

UCLASS()
class AFPSPickableItem : public AFPSPickableBase
{
	GENERATED_BODY()
	
public:
	virtual void OnPicked_Implementation(AActor* Picker) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config")
	class UFPSItemBase* ItemToPick;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (UIMin = 1, ClampMin = 1),Category = "Config")
	int32 Quantity = 1;
};
