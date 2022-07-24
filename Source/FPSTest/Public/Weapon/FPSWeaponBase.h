#pragma once

#include "CoreMinimal.h"
#include "FPSWeaponBase.generated.h"

UCLASS()
class AFPSWeaponBase : public AActor
{
	GENERATED_BODY()
	
public:
	AFPSWeaponBase();
	virtual void Destroyed() override;

	/**Returns target actor for this weapon (create, if its null)*/
	UFUNCTION(BlueprintPure)
	class AFPSTarget_Trace* GetTargetActor();
	UFUNCTION(BlueprintPure)
	USkeletalMeshComponent* GetMesh() const { return WeaponMesh; }
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	USkeletalMeshComponent* WeaponMesh;

	UPROPERTY()
	class AFPSTarget_Trace* TargetActor;
};
