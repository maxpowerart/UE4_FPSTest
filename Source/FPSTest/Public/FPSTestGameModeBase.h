// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "FPSTestGameModeBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FFPSOnPlayerStatusChanged);
UCLASS()
class FPSTEST_API AFPSTestGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (UIMin = 0.f, ClampMin = 0.f), Category = "Config")
	float RespawnTime = 5.f;
	
	UFUNCTION(BlueprintPure)
	FTimerHandle GetRespawnTimerHandle() { return RespawnTimer; }

	UFUNCTION()
	void OnPlayerDied(APlayerController* Player);

	UPROPERTY(BlueprintAssignable)
	FFPSOnPlayerStatusChanged OnPlayerDiedEvent;

	UPROPERTY(BlueprintAssignable)
	FFPSOnPlayerStatusChanged OnPlayerRespawnedEvent;
	
protected:
	UPROPERTY()
	FTimerHandle RespawnTimer;
	UFUNCTION()
	void RespawnPlayer(APlayerController* Player);
};
