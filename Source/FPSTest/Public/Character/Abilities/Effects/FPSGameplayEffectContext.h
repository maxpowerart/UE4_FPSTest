#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "Abilities/GameplayAbilityTargetTypes.h"
#include "FPSGameplayEffectContext.generated.h"

USTRUCT()
struct FPSTEST_API FFPSGameplayEffectContext : public FGameplayEffectContext
{
	GENERATED_USTRUCT_BODY()
	
	virtual FGameplayAbilityTargetDataHandle GetTargetData() const
	{
		return TargetData;
	}

	virtual void AddTargetData(const FGameplayAbilityTargetDataHandle& TargetDataHandle)
	{
		TargetData.Append(TargetDataHandle);
	}
	

	virtual UScriptStruct* GetScriptStruct() const override
	{
		return FFPSGameplayEffectContext::StaticStruct();
	}
	virtual FFPSGameplayEffectContext* Duplicate() const override
	{
		FFPSGameplayEffectContext* NewContext = new FFPSGameplayEffectContext();
		*NewContext = *this;
		NewContext->AddActors(Actors);
		if (GetHitResult())
		{
			// Does a deep copy of the hit result
			NewContext->AddHitResult(*GetHitResult(), true);
		}
		// Shallow copy of TargetData, is this okay?
		NewContext->TargetData.Append(TargetData);
		return NewContext;
	}
	
	virtual bool NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess) override
	{
		return Super::NetSerialize(Ar, Map, bOutSuccess) && TargetData.NetSerialize(Ar, Map, bOutSuccess);
	}

protected:
	FGameplayAbilityTargetDataHandle TargetData;
};

template<>
struct TStructOpsTypeTraits<FFPSGameplayEffectContext> : public TStructOpsTypeTraitsBase2<FFPSGameplayEffectContext>
{
	enum
	{
		WithNetSerializer = true,
		WithCopy = true		// Necessary so that TSharedPtr<FHitResult> Data is copied around
	};
};