#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "FPSPlayerState.generated.h"

UCLASS()
class AFPSPlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:
	AFPSPlayerState();

	UFUNCTION(Exec)
	void Kill();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	class UFPSInventoryComponent* PlayerInventory;
};
