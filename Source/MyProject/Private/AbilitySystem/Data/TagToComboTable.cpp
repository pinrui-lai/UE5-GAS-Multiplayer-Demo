#include "AbilitySystem/Data/TagToComboTable.h"

UComboDataAsset* UTagToComboTable::GetComboTableByTag(FGameplayTag InWeaponTypeTag)
{
	for (auto& TagToComboTable : TagToComboTables)
	{
		if (TagToComboTable.WeaponTypeTag == InWeaponTypeTag)
		{
			return TagToComboTable.ComboDataAsset;
		}
	}
	return nullptr;
}
