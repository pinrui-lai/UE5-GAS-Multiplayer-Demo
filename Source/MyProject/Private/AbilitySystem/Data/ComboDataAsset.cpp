#include "AbilitySystem/Data/ComboDataAsset.h"

FGameplayTag UComboDataAsset::FindAbilityTag(const FGameplayTag InputTag, const FGameplayTag CurrentStateTag)
{
	for (auto It = ComboInfo.begin(); It != ComboInfo.end(); ++It)
	{
		if (CurrentStateTag == It->CurrentStateTag)
		{
			if (InputTag == It->InputTag)
			{
				return It->NextAbilityTag;
			}
		}
	}
	return FGameplayTag();
}

FGameplayTag UComboDataAsset::FindInputTagByAbilityTag(const FGameplayTag InAbilityTag)
{
	for (auto It = ComboInfo.begin(); It != ComboInfo.end(); ++It)
	{
		if (It->NextAbilityTag == InAbilityTag)
		{
			return It->InputTag;
		}
	}
	return FGameplayTag();
}
