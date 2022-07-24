#pragma once

#include "CoreMinimal.h"
#include "Character/FPSCharacterBase.h"
#include "FPSPlayerCharacter.generated.h"

UCLASS()
class AFPSPlayerCharacter : public AFPSCharacterBase
{
	GENERATED_BODY()
	
public:
	AFPSPlayerCharacter();
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	USkeletalMeshComponent* GetFPSMesh() const { return FirstPersonMesh; }
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	class UCameraComponent* FirstPersonCamera;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	class USkeletalMeshComponent* FirstPersonMesh;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	class UFPSEquipmentComponent* Equipment;

protected:
	/**Input delegates*/
	UFUNCTION()
	void MoveForward(float Value);
	UFUNCTION()
	void MoveRight(float Value);
	UFUNCTION()
	void LookUp(float Value);
	UFUNCTION()
	void Turn(float Value);
};
