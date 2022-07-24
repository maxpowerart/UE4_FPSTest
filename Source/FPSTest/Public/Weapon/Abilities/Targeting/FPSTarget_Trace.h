// Copyright 2020 Dan Kestranek.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbilityTargetActor.h"
#include "Components/TimelineComponent.h"
#include "Engine/CollisionProfile.h"
#include "FPSTarget_Trace.generated.h"

UCLASS()
class FPSTEST_API AFPSTarget_Trace : public AGameplayAbilityTargetActor
{
	GENERATED_BODY()
	
public:
    AFPSTarget_Trace();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void StartTargeting(UGameplayAbility* Ability) override;
	virtual void ConfirmTargetingAndContinue() override;
	
	// Current spread from continuous targeting
	float CurrentTargetingSpread = -1.f;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, config, meta = (ExposeOnSpawn = true), Category = "Trace")
	FCollisionProfileName TraceProfile;

protected:
	/**Returns result with all target data per shot*/
	virtual TArray<FHitResult> PerformTrace(AActor* InSourceActor);

	UPROPERTY()
	class UFPSEquipmentComponent* EquipmentInfo;
	UPROPERTY()
	class UAbilitySystemComponent* ASC;

	UPROPERTY()
	FTimerHandle DelayTimerHandle;
	UFUNCTION()
	void BeginDescreaseSpread();

	UPROPERTY()
	FTimeline DecreaseSpreadTimeline;
	UPROPERTY()
	UCurveFloat* LinearCurve;
	UFUNCTION()
	void DecreaseSpread_Tick(float Value);
	UFUNCTION()
	void DecreaseSpread_Done();
	float StartValue;
};
