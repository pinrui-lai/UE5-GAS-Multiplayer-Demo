#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MyAbilitySystemLibrary.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API UMyAbilitySystemLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintPure, Category = "MyAbilitySystemLibrary|EffectContext")
	static bool EffectContext_IsCriticalHit(const FGameplayEffectContextHandle& EffectContextHandle);
	
	UFUNCTION(BlueprintPure, Category = "MyAbilitySystemLibrary|EffectContext")
	static void EffectContext_SetIsCriticalHit(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, bool bInIsCriticalHit);
	
	UFUNCTION(BlueprintPure, Category = "MyAbilitySystemLibrary|EffectContext")
	static bool EffectContext_IsCountered(const FGameplayEffectContextHandle& EffectContextHandle);
	
	UFUNCTION(BlueprintPure, Category = "MyAbilitySystemLibrary|EffectContext")
	static void EffectContext_SetIsCountered(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, bool bInIsCountered);
	
	UFUNCTION(BlueprintPure, Category = "MyAbilitySystemLibrary|EffectContext")
	static bool EffectContext_IsBlocked(const FGameplayEffectContextHandle& EffectContextHandle);
	
	UFUNCTION(BlueprintPure, Category = "MyAbilitySystemLibrary|EffectContext")
	static void EffectContext_SetIsBlocked(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, bool bInIsBlocked);
};
