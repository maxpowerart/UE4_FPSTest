#pragma once

#include "CoreMinimal.h"
#include "GameplayCueNotify_Static.h"
#include "FPSCue_Fire.generated.h"

UCLASS()
class UFPSCue_Fire : public UGameplayCueNotify_Static
{
	GENERATED_BODY()
	
public:
	UFPSCue_Fire();
	virtual bool OnExecute_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters) const override;
};
