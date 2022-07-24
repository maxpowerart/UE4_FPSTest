#include "Pickable/FPSPickableBase.h"

#include "Components/SphereComponent.h"

AFPSPickableBase::AFPSPickableBase()
{
	Root = CreateDefaultSubobject<USceneComponent>("Root");
	SetRootComponent(Root);

	Collider = CreateDefaultSubobject<USphereComponent>("Collider");
	Collider->SetupAttachment(GetRootComponent());
	Collider->OnComponentBeginOverlap.AddDynamic(this, &AFPSPickableBase::OnColliderTriggered);
}

void AFPSPickableBase::OnPicked_Implementation(AActor* Picker)
{
	Destroy();
}

void AFPSPickableBase::OnColliderTriggered(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	OnPicked(OtherActor);
}
