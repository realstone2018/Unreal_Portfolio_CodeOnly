#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PTPlayerController.generated.h"

UCLASS()
class PROJECT2_API APTPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	APTPlayerController();

	virtual void BeginPlay() override;

	void ReSetupHUDWidget();
	
	FORCEINLINE void GameClear() { K2_OnGameClear(); }
	FORCEINLINE void GameOver() { K2_OnGameOver(); }
	FORCEINLINE void TimerStart() { K2_OnTimerStart(); }
	FORCEINLINE void PlayerRespawnTimerStart() { K2_OnPlayerRespawnTimerStart(); }
	
protected:
	UFUNCTION(BlueprintImplementableEvent, Category = UE_GAME, Meta = (DisplayName = "OnGameClearCpp"))
	void K2_OnGameClear();

	UFUNCTION(BlueprintImplementableEvent, Category = UE_GAME, Meta = (DisplayName = "OnGameOverCpp"))
	void K2_OnGameOver();

	UFUNCTION(BlueprintImplementableEvent, Category = UE_GAME, Meta = (DisplayName = "OnTimerStartCpp"))
	void K2_OnTimerStart();

	UFUNCTION(BlueprintImplementableEvent, Category = UE_GAME, Meta = (DisplayName = "OnPlayerRespawnTimerStartCpp"))
	void K2_OnPlayerRespawnTimerStart();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HUD)
	TSubclassOf<class UPTHUDWidget> HUDClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = HUD)
	TObjectPtr<class UPTHUDWidget> HUD;
};
