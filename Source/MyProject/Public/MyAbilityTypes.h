#pragma once

# include "GameplayEffectTypes.h"
# include "MyAbilityTypes.generated.h"

USTRUCT(BlueprintType)
struct FMyGameplayEffectContext : public FGameplayEffectContext
{
	GENERATED_BODY()
	
public:
	bool IsCriticalHit() const {return bIsCriticalHit;}
	bool IsCountered() const {return bIsCountered;}
	bool IsBlocked() const {return bIsBlocked;}
	
	void SetIsCriticalHit(const bool bInIsCriticalHit) {bIsCriticalHit = bInIsCriticalHit;}
	void SetIsCountered(const bool bInIsCountered) {bIsCountered = bInIsCountered;}
	void SetIsBlocked(const bool bInIsBlocked) {bIsBlocked = bInIsBlocked;}
	
	virtual UScriptStruct* GetScriptStruct() const override
	{
		return StaticStruct();
	}
	
	virtual FMyGameplayEffectContext* Duplicate() const override
	{
		FMyGameplayEffectContext* NewContext = new FMyGameplayEffectContext();
		*NewContext = *this;
		if (GetHitResult())
		{
			// Does a deep copy of the hit result
			NewContext->AddHitResult(*GetHitResult(), true);
		}
		return NewContext;
	}
	
	virtual bool NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess) override;
	
private:
	UPROPERTY()
	bool bIsCriticalHit = false;
	
	UPROPERTY()
	bool bIsCountered = false;
	
	UPROPERTY()
	bool bIsBlocked = false;
};

template<>
struct TStructOpsTypeTraits<FMyGameplayEffectContext> : public TStructOpsTypeTraitsBase2<FMyGameplayEffectContext>
{
	enum
	{
		WithNetSerializer = true,
		WithCopy = true,
	};
};
