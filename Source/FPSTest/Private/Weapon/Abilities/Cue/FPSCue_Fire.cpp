#include "Weapon/Abilities/Cue/FPSCue_Fire.h"

#include "Character/Abilities/Effects/FPSGameplayEffectContext.h"
#include "Equipment/FPSEquipmentComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"

UFPSCue_Fire::UFPSCue_Fire()
{
	GameplayCueTag = FGameplayTag::RequestGameplayTag(FName("GameplayCue.Weapon.Fire"));
}

bool UFPSCue_Fire::OnExecute_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters) const
{
	if(!IsValid(MyTarget)) return false;
	
	const UFPSEquipmentComponent* Equipment = Cast<UFPSEquipmentComponent>(MyTarget->GetComponentByClass(UFPSEquipmentComponent::StaticClass()));
	if(!IsValid(Equipment)) return false;

	const UFPSWeaponDataAsset* WeaponDA = Equipment->GetCurrentSlot().WeaponDA;
	const AFPSWeaponBase* Weapon = Equipment->GetCurrentSlot().WeaponInstance;
	if(!IsValid(Weapon) || !IsValid(WeaponDA)) return false;

	/**Apply camera shake for player*/
	APlayerController* PC = Cast<APlayerController>(Cast<APawn>(MyTarget)->GetController());
	if(IsValid(PC) && IsValid(WeaponDA->CameraShakeClass))
	{
		PC->ClientStartCameraShake(WeaponDA->CameraShakeClass);
	}

	/**Muzzle particle*/
	if(IsValid(WeaponDA->MuzzleEmitter))
	{
		UGameplayStatics::SpawnEmitterAttached(WeaponDA->MuzzleEmitter, Weapon->GetMesh(), WeaponDA->MuzzleSocket);
	}
	
	const FFPSGameplayEffectContext* EffectContext = static_cast<const FFPSGameplayEffectContext*>(Parameters.EffectContext.Get());
	const FGameplayAbilityTargetDataHandle Handle = EffectContext->GetTargetData();

	/**Trail particle for each shot*/
	if(IsValid(WeaponDA->TrailEmitter))
	{
		for(int32 ShotNum = 0; ShotNum < Handle.Num(); ShotNum++)
		{
			UParticleSystemComponent* Particle = UGameplayStatics::SpawnEmitterAtLocation(MyTarget, WeaponDA->TrailEmitter,
				Handle.Data[ShotNum].Get()->GetHitResult()->TraceStart);
			Particle->SetVectorParameter(FName("TraceEnd"), Handle.Data[ShotNum].Get()->GetHitResult()->ImpactPoint);
		}
	}

	/**Sound*/
	if(IsValid(WeaponDA->ShotSound))
	{
		UGameplayStatics::PlaySoundAtLocation(MyTarget, WeaponDA->ShotSound, Handle.Data[0].Get()->GetHitResult()->TraceStart);
	}

	return Super::OnExecute_Implementation(MyTarget, Parameters);
}
