#include "UI/PTHUDWidget.h"
#include "PTHpBarWidget.h"
#include "PTCharacterStatWidget.h"
#include "PTEquipWeaponWidget.h"
#include "PTGunInfoWidget.h"
#include "PTInterface/PTHUDInterface.h"

void UPTHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

	HpBar = Cast<UPTHpBarWidget>(GetWidgetFromName(TEXT("WidgetHpBar")));
	ensure(HpBar);

	CharacterStat = Cast<UPTCharacterStatWidget>(GetWidgetFromName("WidgetCharacterStat"));
	ensure(CharacterStat);

	GunInfo = Cast<UPTGunInfoWidget>(GetWidgetFromName("WidgetGunInfo"));
	ensure(GunInfo);

	EquipWeapon = Cast<UPTEquipWeaponWidget>(GetWidgetFromName("WidgetEquipWeapon"));
	ensure(EquipWeapon);
	
	IPTHUDInterface* HUDPawn = Cast<IPTHUDInterface>(GetOwningPlayerPawn());
	if (HUDPawn)
	{
		HUDPawn->SetupHUDWidget(this);	
	}
}

void UPTHUDWidget::UpdateStat(const FPTCharacterStat& BaseStat, const FPTCharacterStat& ModifierStat)
{
	FPTCharacterStat TotalStat = BaseStat + ModifierStat;
	HpBar->SetMaxHp(TotalStat.MaxHp); 

	CharacterStat->UpdateStat(BaseStat, ModifierStat);
}

void UPTHUDWidget::UpdateHpBar(int32 NewCurrentHp)
{
	HpBar->UpdateHpBar(NewCurrentHp);
}

void UPTHUDWidget::UpdateGunAmmo(int32 NewCurAmmo, int32 NewMaxAmmo)
{
	GunInfo->UpdateBullet(NewCurAmmo, NewMaxAmmo);
}

void UPTHUDWidget::UpdateGunReloadImg(uint8 InVisibility)
{
	GunInfo->SetReloadingImg(InVisibility);
}

void UPTHUDWidget::UpdateEquipWeapon(uint8 IsEquipMain)
{
	IsEquipMain ? EquipWeapon->EquipMainWeapon() : EquipWeapon->EquipSubWeapon();
}

