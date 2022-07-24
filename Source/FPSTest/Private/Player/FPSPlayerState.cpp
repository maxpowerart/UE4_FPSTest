#include "Player/FPSPlayerState.h"

#include "FPSTestGameModeBase.h"
#include "Inventory/FPSInventoryComponent.h"
#include "Kismet/GameplayStatics.h"

AFPSPlayerState::AFPSPlayerState()
{
	PlayerInventory = CreateDefaultSubobject<UFPSInventoryComponent>("PlayerInventory");
}

void AFPSPlayerState::Kill()
{
	Cast<AFPSTestGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()))->OnPlayerDied(Cast<APlayerController>(GetPawn()->Controller));
}
