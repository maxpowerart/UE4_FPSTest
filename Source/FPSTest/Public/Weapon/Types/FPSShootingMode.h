#pragma once

#include "CoreMinimal.h"
#include "FPSShootingMode.generated.h"

UENUM(BlueprintType)
enum EFPSShootingMode
{
	/**One bullet per click*/
	Single,

	/**Some amount of bullets per click*/
	Burst,

	/**All bullets per click*/
	FullAuto
};
