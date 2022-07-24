// Copyright Epic Games, Inc. All Rights Reserved.

#include "FPSTestGameModeBase.h"
#include "GameFramework/SpectatorPawn.h"
#include "Player/FPSPlayerCharacter.h"
#include "Player/FPSPlayerState.h"

void AFPSTestGameModeBase::OnPlayerDied(APlayerController* Player)
{
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	ASpectatorPawn* SpectatorPawn = GetWorld()->SpawnActor<ASpectatorPawn>(SpectatorClass, Player->GetPawn()->GetActorTransform(), SpawnParameters);

	Cast<AFPSPlayerCharacter>( Player->GetPawn())->OnKilled();
	Player->UnPossess();
	Player->Possess(SpectatorPawn);
	
	FTimerDelegate RespawnDelegate = FTimerDelegate::CreateUObject(this, &AFPSTestGameModeBase::RespawnPlayer, Player);
	GetWorldTimerManager().SetTimer(RespawnTimer, RespawnDelegate, RespawnTime, false);

	OnPlayerDiedEvent.Broadcast();
}

void AFPSTestGameModeBase::RespawnPlayer(APlayerController* Player)
{
	const AActor* PlayerStart = FindPlayerStart(Player);
	AFPSPlayerCharacter* Hero = GetWorld()->SpawnActorDeferred<AFPSPlayerCharacter>(DefaultPawnClass,
		FTransform(PlayerStart->GetActorRotation(),PlayerStart->GetActorLocation()), Player,
		nullptr, ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn);

	Hero->SetPlayerState(Player->GetPlayerState<AFPSPlayerState>());
	Hero->FinishSpawning(FTransform(PlayerStart->GetActorRotation(), PlayerStart->GetActorLocation()));
	
	APawn* OldSpectatorPawn = Player->GetPawn();
	Player->UnPossess();
	OldSpectatorPawn->Destroy();
	Player->Possess(Hero);

	OnPlayerRespawnedEvent.Broadcast();
}
