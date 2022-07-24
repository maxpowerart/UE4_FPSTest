#pragma once

#include "CoreMinimal.h"
#include "FPSPickableBase.generated.h"

UCLASS()
class AFPSPickableBase : public AActor
{
	GENERATED_BODY()
	
public:
	/**Constructor and overrides*/
	AFPSPickableBase();

	UFUNCTION(BlueprintNativeEvent, Category = "Events")
	void OnPicked(AActor* Picker);
	virtual void OnPicked_Implementation(AActor* Picker);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	USceneComponent* Root;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	class USphereComponent* Collider;

	/**Delegate for collider. Can be modified in child classes for unusual cases*/
	UFUNCTION()
	virtual void OnColliderTriggered(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
