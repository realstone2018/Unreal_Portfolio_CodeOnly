#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Manager/PTObjectPoolManager.h"
#include "PTSpawnManager.generated.h"

UCLASS()
class PROJECT2_API UPTSpawnManager : public UObject
{
	GENERATED_BODY()

public:
	void Init(UWorld* InWorld, UPTObjectPoolManager* InPoolManager);

#pragma region Polling
public:
	template <typename T, typename = typename TEnableIf<TIsDerivedFrom<T, IPTPullingObjectInterface>::IsDerived>::Type>
	T* SpawnObject(FName DataKey, FRotator SpawnRotator, FVector SpawnLocation, uint8 ReturnImmediately);

	template <typename T, typename = typename TEnableIf<TIsDerivedFrom<T, IPTPullingObjectInterface>::IsDerived>::Type>
	void ReturnObject(AActor* PooledObject);

	template <typename T, typename = typename TEnableIf<TIsDerivedFrom<T, IPTPullingObjectInterface>::IsDerived>::Type>
	void ReturnImmediatelyObject(AActor* PooledObject);
	
private:
	UPROPERTY()
	TObjectPtr<class UWorld> World;
	
	UPROPERTY()
	TObjectPtr<UPTObjectPoolManager> PoolManager;

	const float SPAWN_RADIUS = 8000.f;
#pragma endregion


#pragma region Monster
public:
	void SpawnMonsterWave(FVector BaseSpawnLocation, int32 Num);


#pragma endregion
};
