#include "Manager/PTObjectPoolManager.h"
#include "PTInterface/PTPullingObjectInterface.h"
#include "Containers/Map.h"
#include "Containers/Array.h"
#include "Engine/AssetManager.h"
#include "GameData/ObjectPoolData.h"
#include "Enum/ObjectPoolType.h"

UPTObjectPoolManager::UPTObjectPoolManager()
{
	static ConstructorHelpers::FObjectFinder<UObjectPoolData> PoolDataRef(TEXT("/Script/Project2.ObjectPoolData'/Game/Project2/GameData/ObjectPoolDataAsset.ObjectPoolDataAsset'"));
	if (PoolDataRef.Object)
	{
		PoolData = PoolDataRef.Object;
	}
}

void UPTObjectPoolManager::Init(UWorld* World)
{
	WorldContext = World;
}

template <typename T, typename>
void UPTObjectPoolManager::SetUpPool(EPoolType PoolType, /*TSubclassOf<T> ObjectClass,*/ int32 SetUpSize)
{
	FString ParentName = UEnum::GetValueAsString(PoolType);
	FString ParentPath = "ObjectPool/" + ParentName;
	
	if (!PoolMap.Contains(PoolType))
	{
		PoolMap.Add(PoolType, FPoolArray());
	}

	ensure(PoolData);
	const FPoolData Data = PoolData->GetPoolData(PoolType);
	int32 Size = SetUpSize != 0 ? SetUpSize : Data.SetupSize;
	FVector SpawnLocation(0.f, 0.f, 5000.f);
	
	for (int32 i = 0; i < Size; i++)
	{
		UE_LOG(LogTemp, Error, TEXT("Set Up Pool is Fail, PoolType: %s   ClassName: %s"), *UEnum::GetValueAsString(PoolType), *Data.PoolClass->GetName());

		AActor* NewObject = WorldContext->SpawnActor<T>(Data.PoolClass, SpawnLocation, FRotator::ZeroRotator);
		if (NewObject == nullptr)
		{
			UE_LOG(LogTemp, Error, TEXT("Set Up Pool is Fail, PoolType: %s"), *UEnum::GetValueAsString(PoolType));
			ensure(NewObject);
			continue;
		}
		
		NewObject->SetFolderPath(*ParentPath);
		if (IPTPullingObjectInterface* PoolableObject = Cast<IPTPullingObjectInterface>(NewObject))
		{
			PoolableObject->Terminate();
			PoolMap[PoolType].DeactiveArray.Add(NewObject);
		}
	}
}

template <typename T, typename>
T* UPTObjectPoolManager::GetPooledObject(FTransform const& Trans, FName DataKey)
{
	EPoolType PoolType = EPoolTypeUtil::ClassToPoolType(T::StaticClass());
	if (PoolType == EPoolType::None)
	{
		return nullptr;
	}
	
	if (!PoolMap.Contains(PoolType))
	{
		return nullptr;
	}

	if (PoolMap[PoolType].DeactiveArray.Num() <= 0)
	{
		SetUpPool<T>(PoolType, PoolMap[PoolType].ActiveArray.Num() / 2);
		UE_LOG(LogTemp, Error, TEXT("%s PoolType is Full, Increase Pull Size"), *UEnum::GetValueAsString(PoolType));
	}
	
	AActor* Object = PoolMap[PoolType].DeactiveArray[0];
	PoolMap[PoolType].DeactiveArray.RemoveAt(0);
	PoolMap[PoolType].ActiveArray.Add(Object);

	Object->SetActorTransform(Trans);

	IPTPullingObjectInterface* InterfaceObject = Cast<IPTPullingObjectInterface>(Object);
	InterfaceObject->Initialize(DataKey);

	T* PoolableObject = Cast<T>(Object);
	check(PoolableObject);
	
	return PoolableObject;
}

template <typename T, typename>
void UPTObjectPoolManager::ReturnPooledObject(AActor* Object)
{
	EPoolType PoolType = EPoolTypeUtil::ClassToPoolType(T::StaticClass());
	if (PoolType == EPoolType::None)
	{
		return;
	}
		
	if (IPTPullingObjectInterface* PoolableObject = Cast<IPTPullingObjectInterface>(Object))
	{
		PoolableObject->Terminate();

		PoolMap[PoolType].DeactiveArray.Add(Object);
		PoolMap[PoolType].ActiveArray.Remove(Object);
	}
}
