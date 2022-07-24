#include "Weapon/FPSWeaponBase.h"

#include "Weapon/Abilities/Targeting/FPSTarget_Trace.h"

AFPSWeaponBase::AFPSWeaponBase()
{
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>("WeaponMesh");
	SetRootComponent(WeaponMesh);
}

void AFPSWeaponBase::Destroyed()
{
	if(IsValid(TargetActor)) TargetActor->Destroy();
	Super::Destroyed();
}

AFPSTarget_Trace* AFPSWeaponBase::GetTargetActor()
{
	if (IsValid(TargetActor))
	{
		return TargetActor;
	}

	TargetActor = GetWorld()->SpawnActor<AFPSTarget_Trace>();
	TargetActor->SetOwner(this);
	return TargetActor;
}
