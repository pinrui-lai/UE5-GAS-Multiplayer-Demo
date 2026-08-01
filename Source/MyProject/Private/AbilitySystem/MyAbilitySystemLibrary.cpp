#include "AbilitySystem/MyAbilitySystemLibrary.h"

#include "MyAbilityTypes.h"

bool UMyAbilitySystemLibrary::EffectContext_IsCriticalHit(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FMyGameplayEffectContext* MyContext = static_cast<const FMyGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return MyContext->IsCriticalHit();
	}
	return false;
}

void UMyAbilitySystemLibrary::EffectContext_SetIsCriticalHit(FGameplayEffectContextHandle& EffectContextHandle,
	const bool bInIsCriticalHit)
{
	if (FMyGameplayEffectContext* MyContext = static_cast<FMyGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		MyContext->SetIsCriticalHit(bInIsCriticalHit);
	}
}

bool UMyAbilitySystemLibrary::EffectContext_IsCountered(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FMyGameplayEffectContext* MyContext = static_cast<const FMyGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return MyContext->IsCountered();
	}
	return false;
}

void UMyAbilitySystemLibrary::EffectContext_SetIsCountered(FGameplayEffectContextHandle& EffectContextHandle,
	const bool bInIsCountered)
{
	if (FMyGameplayEffectContext* MyContext = static_cast<FMyGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		MyContext->SetIsCountered(bInIsCountered);
	}
}

bool UMyAbilitySystemLibrary::EffectContext_IsBlocked(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FMyGameplayEffectContext* MyContext = static_cast<const FMyGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return MyContext->IsBlocked();
	}
	return false;
}

void UMyAbilitySystemLibrary::EffectContext_SetIsBlocked(FGameplayEffectContextHandle& EffectContextHandle,
	const bool bInIsBlocked)
{
	if (FMyGameplayEffectContext* MyContext = static_cast<FMyGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		MyContext->SetIsBlocked(bInIsBlocked);
	}
}
