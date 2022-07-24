#pragma once

#include "CoreMinimal.h"
#include "Pickable/FPSPickableItem.h"
#include "FPSPickableWeapon.generated.h"

UCLASS()
class AFPSPickableWeapon : public AFPSPickableItem
{
	GENERATED_BODY()
	
public:
	/**Here we can add some external logic, like saving ammo after drop*/
	virtual void OnPicked_Implementation(AActor* Picker) override;
};