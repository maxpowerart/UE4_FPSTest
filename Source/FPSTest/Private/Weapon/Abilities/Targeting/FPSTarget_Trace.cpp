#include "Weapon/Abilities/Targeting/FPSTarget_Trace.h"

#include "DrawDebugHelpers.h"
#include "Abilities/GameplayAbility.h"
#include "Character/FPSAbilitySystemComponent.h"
#include "Equipment/FPSEquipmentComponent.h"
#include "Kismet/KismetMathLibrary.h"

AFPSTarget_Trace::AFPSTarget_Trace()
{
	bDestroyOnConfirmation = false;
	PrimaryActorTick.bCanEverTick = true;
}

void AFPSTarget_Trace::BeginPlay()
{
	Super::BeginPlay();

	FRichCurve* SmoothCurve = new FRichCurve();
	SmoothCurve->AddKey(0.0, 0.0);
	SmoothCurve->AddKey(1.0, 1.0);
	
	LinearCurve = NewObject<UCurveFloat>();
	LinearCurve->FloatCurve = *SmoothCurve;

	/**Set zoom timeline*/
	FOnTimelineFloat DecreaseSpreadProgress;
	DecreaseSpreadProgress.BindUFunction(this, FName("DecreaseSpread_Tick"));
	DecreaseSpreadTimeline.AddInterpFloat(LinearCurve, DecreaseSpreadProgress);
	DecreaseSpreadTimeline.SetTimelineLength(1);

	FOnTimelineEvent DecreaseSpreadEnd;
	DecreaseSpreadEnd.BindUFunction(this, FName("DecreaseSpread_Done"));
	DecreaseSpreadTimeline.SetTimelineFinishedFunc(DecreaseSpreadEnd);

	SetActorTickEnabled(false);
}

void AFPSTarget_Trace::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if(DecreaseSpreadTimeline.IsPlaying()) DecreaseSpreadTimeline.TickTimeline(DeltaSeconds);
}

void AFPSTarget_Trace::StartTargeting(UGameplayAbility* Ability)
{
	Super::StartTargeting(Ability);
	EquipmentInfo = Cast<UFPSEquipmentComponent>(Ability->GetCurrentActorInfo()->AvatarActor.Get()->GetComponentByClass(UFPSEquipmentComponent::StaticClass()));
	SourceActor = Ability->GetCurrentActorInfo()->AvatarActor.Get();
	ASC = Ability->GetAbilitySystemComponentFromActorInfo();
	MasterPC = Cast<APlayerController>(Cast<APawn>(SourceActor)->GetController());
	StartLocation.LocationType = EGameplayAbilityTargetingLocationType::SocketTransform;
	StartLocation.SourceComponent = EquipmentInfo->GetCurrentSlot().WeaponInstance->GetMesh();
	StartLocation.SourceSocketName = EquipmentInfo->GetCurrentSlot().WeaponDA->MuzzleSocket;

	const bool bAiming = Cast<UFPSAbilitySystemComponent>(OwningAbility->GetAbilitySystemComponentFromActorInfo())->IsActiveAbilityWithTag(
		FGameplayTag::RequestGameplayTag(FName("Ability.Weapon.Aim")));
	
	CurrentTargetingSpread = FMath::Max(EquipmentInfo->GetCurrentSlot().WeaponDA->WeaponSpread *
		(bAiming ? EquipmentInfo->GetCurrentSlot().WeaponDA->AimSpreadingMultiplier : 1.f),
		CurrentTargetingSpread);
}

void AFPSTarget_Trace::ConfirmTargetingAndContinue()
{
	if(IsValid(SourceActor))
	{
		/**Make data handle*/
		TArray<FHitResult> HitResults = PerformTrace(SourceActor);
		FGameplayAbilityTargetDataHandle DataHandle;
		for (int32 i = 0; i < HitResults.Num(); i++)
		{
			/** Note: These are cleaned up by the FGameplayAbilityTargetDataHandle (via an internal TSharedPtr) */
			FGameplayAbilityTargetData_SingleTargetHit *ReturnData = new FGameplayAbilityTargetData_SingleTargetHit();
			ReturnData->HitResult = HitResults[i];
			DataHandle.Add(ReturnData);
		}
		
		TargetDataReadyDelegate.Broadcast(DataHandle);
	}
}

TArray<FHitResult> AFPSTarget_Trace::PerformTrace(AActor* InSourceActor)
{
	constexpr bool bTraceComplex = false;
	TArray<AActor *> ActorsToIgnore;
	ActorsToIgnore.Add(InSourceActor);

	FCollisionQueryParams Params(SCENE_QUERY_STAT(AGSGATA_LineTrace), bTraceComplex);
	Params.bReturnPhysicalMaterial = true;
	Params.AddIgnoredActors(ActorsToIgnore);
	Params.bIgnoreBlocks = false;

	const FVector TraceStart = StartLocation.GetTargetingTransform().GetLocation();

	/**Find target location by trace from player's view*/
	FVector ViewLocation;
	FRotator ViewRotation;
	MasterPC->GetPlayerViewPoint(ViewLocation, ViewRotation);
	FHitResult Result;
	GetWorld()->LineTraceSingleByChannel(Result, ViewLocation, ViewLocation + ViewRotation.Vector().GetSafeNormal() * 99999, ECollisionChannel::ECC_Visibility);
	const FVector GeneralDirection = Result.TraceEnd - StartLocation.GetTargetingTransform().GetLocation().GetSafeNormal();

	/**Get bullets num*/
	const int32 BulletsNum = EquipmentInfo->GetCurrentSlot().WeaponDA->bRandomBulletQuantity ?
		FMath::RandRange(EquipmentInfo->GetCurrentSlot().WeaponDA->BulletsAmount.GetLowerBoundValue(),
			EquipmentInfo->GetCurrentSlot().WeaponDA->BulletsAmount.GetUpperBoundValue()) : EquipmentInfo->GetCurrentSlot().WeaponDA->BulletsAmount.GetUpperBoundValue();

	/**Make all bullets' traces from muzzle*/
	TArray<FHitResult> ReturnHitResults;
	for (int32 TraceIndex = 0; TraceIndex < BulletsNum; TraceIndex++)
	{
		/**SetTraceEnd with rand in range*/
		const float ConeHalfAngle = FMath::DegreesToRadians(CurrentTargetingSpread * 0.5f);
		const int32 RandomSeed = FMath::Rand();
		FRandomStream WeaponRandomStream(RandomSeed);
		const FVector ShootDir = WeaponRandomStream.VRandCone(GeneralDirection, ConeHalfAngle, ConeHalfAngle);
		const FVector TraceEnd = TraceStart + (ShootDir * 999999);
		
		TArray<FHitResult> TraceHitResults;
		GetWorld()->LineTraceMultiByChannel(TraceHitResults, TraceStart, TraceEnd, ECollisionChannel::ECC_Visibility);

		/**Add default for empty (for example, shooting in the sky case)*/
		if(TraceHitResults.Num() < 1)
		{
			FHitResult HitResult;
			HitResult.TraceStart = StartLocation.GetTargetingTransform().GetLocation();
			HitResult.TraceEnd = TraceEnd;
			HitResult.Location = TraceEnd;
			HitResult.ImpactPoint = TraceEnd;
			TraceHitResults.Add(HitResult);
		}

		ReturnHitResults.Append(TraceHitResults);
	}	

	/**Increment current spread*/
	const bool bAiming = Cast<UFPSAbilitySystemComponent>(OwningAbility->GetAbilitySystemComponentFromActorInfo())->IsActiveAbilityWithTag(
		FGameplayTag::RequestGameplayTag(FName("Ability.Weapon.Aim")));
	CurrentTargetingSpread = FMath::Min(EquipmentInfo->GetCurrentSlot().WeaponDA->WeaponSpreadMax,
		CurrentTargetingSpread + EquipmentInfo->GetCurrentSlot().WeaponDA->WeaponSpreadIncrement) *
			(bAiming ? EquipmentInfo->GetCurrentSlot().WeaponDA->AimSpreadingMultiplier : 1.f);

	GetWorld()->GetTimerManager().SetTimer(DelayTimerHandle, this, &AFPSTarget_Trace::BeginDescreaseSpread, EquipmentInfo->GetCurrentSlot().WeaponDA->DecreaseSpreadDelay);
	
	return ReturnHitResults;
}

void AFPSTarget_Trace::BeginDescreaseSpread()
{
	/**Set timeline to decrease spread*/
	StartValue = CurrentTargetingSpread;

	const float DecreaseRate = 1.f /
		(CurrentTargetingSpread / ((EquipmentInfo->GetCurrentSlot().WeaponDA->WeaponSpreadMax - EquipmentInfo->GetCurrentSlot().WeaponDA->WeaponSpread) /
			EquipmentInfo->GetCurrentSlot().WeaponDA->DecreaseSpreadTime));
	
	DecreaseSpreadTimeline.SetPlayRate(DecreaseRate);
	DecreaseSpreadTimeline.PlayFromStart();
	SetActorTickEnabled(true);
}

void AFPSTarget_Trace::DecreaseSpread_Tick(float Value)
{
	if(!IsValid(OwningAbility) || !IsValid(EquipmentInfo) || !IsValid(ASC))
	{
		DecreaseSpreadTimeline.Stop();
		DecreaseSpread_Done();
		return;
	}
		
	const bool bAiming = Cast<UFPSAbilitySystemComponent>(OwningAbility->GetAbilitySystemComponentFromActorInfo())->IsActiveAbilityWithTag(
		FGameplayTag::RequestGameplayTag(FName("Ability.Weapon.Aim")));
	
	CurrentTargetingSpread = FMath::Lerp(StartValue, EquipmentInfo->GetCurrentSlot().WeaponDA->WeaponSpread, Value) *
		(bAiming ? EquipmentInfo->GetCurrentSlot().WeaponDA->AimSpreadingMultiplier : 1.f);
	
}
void AFPSTarget_Trace::DecreaseSpread_Done()
{
	SetActorTickEnabled(false);
}
