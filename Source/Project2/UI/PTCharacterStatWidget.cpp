#include "UI/PTCharacterStatWidget.h"
#include "Components/TextBlock.h"

void UPTCharacterStatWidget::NativeConstruct()
{
	Super::NativeConstruct();

	for (TFieldIterator<FNumericProperty> PropIt(FPTCharacterStat::StaticStruct()); PropIt; ++PropIt)
	{
		const FName PropKey(PropIt->GetName());
		const FName TextBaseControlName = *FString::Printf(TEXT("Txt%sBase"), *PropIt->GetName());
		const FName TextModifierControlName = *FString::Printf(TEXT("Txt%sModifier"), *PropIt->GetName());

		UTextBlock* BaseTextBlock = Cast<UTextBlock>(GetWidgetFromName(TextBaseControlName));
		if (BaseTextBlock)
		{
			BaseLookup.Add(PropKey, BaseTextBlock);	
		}

		UTextBlock* ModifierTextBlock = Cast<UTextBlock>(GetWidgetFromName(TextModifierControlName));
		if (ModifierTextBlock)
		{
			ModifierLookup.Add(PropKey, ModifierTextBlock);
		}
	}	
}

void UPTCharacterStatWidget::UpdateStat(const FPTCharacterStat& BaseStat, const FPTCharacterStat& ModifierStat)
{
	for (TFieldIterator<FNumericProperty> PropIt(FPTCharacterStat::StaticStruct()); PropIt; ++PropIt)
	{
		const FName PropKey(PropIt->GetName());

		UTextBlock** BaseTextBlockPtr = BaseLookup.Find(PropKey);
		UTextBlock** ModifierTextBlockPtr = ModifierLookup.Find(PropKey);

		if (PropIt->IsFloatingPoint())
		{
			float BaseData = 0.0f;
			PropIt->GetValue_InContainer((const void*)&BaseStat, &BaseData);
			float ModifierData = 0.0f;
			PropIt->GetValue_InContainer((const void*)&ModifierStat, &ModifierData);

			(*BaseTextBlockPtr)->SetText(FText::FromString(FString::SanitizeFloat(BaseData)));
			(*ModifierTextBlockPtr)->SetText(FText::FromString(FString::SanitizeFloat(ModifierData)));
		}
		else if (PropIt->IsInteger())
		{
			int32 BaseData = 0;
			PropIt->GetValue_InContainer((const void*)&BaseStat, &BaseData);
			float ModifierData = 0.0f;
			PropIt->GetValue_InContainer((const void*)&ModifierStat, &ModifierData);

			(*BaseTextBlockPtr)->SetText(FText::FromString(FString::FromInt(BaseData)));
			(*ModifierTextBlockPtr)->SetText(FText::FromString(FString::FromInt(ModifierData)));
		}
	}	
}
