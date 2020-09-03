// Copyright (c) 2020 Jonas Reich

#pragma once

#include "CoreMinimal.h"
/*
USTRUCT()
struct FWorldObjectPoolSettings
{
	UPROPERTY(EditDefaultsOnly, meta = (MustImplement = "WorldObjectPoolFactory"))
	TSubclassOf<IWorldObjectPoolFactory> FactoryClass;
	 
	// pool settings
	// Hard limits:
	int32 MinSize = 10;
	int32 MaxSize = 300;

	// Min is always more important than max:
	int32 MinSlack = 5;
	int32 MaxSlack = 50;
	float MinSlackPercentage = 0.1f;
	float MaxSlackPercentage = 0.5f;

};

template<typename T, typename InstanceParams>
class IWorldObjectPoolFactory
{
	void CreateInstance(T*);
	void InitializeInstance(T*, InstanceParams p) const;
	void ResetInstance(T*) const;
	void PreDestroyInstance(T*) const;
	void DestroyInstance(T*);
};

class USimpleActorFactory : 

template<typename T, typename InstanceParams>
class TWorldObjectPool
{
	static_assert(TIsDerivedFrom<T, UObject>::Value, "T must be derived from UObject!");

	UObject* OuterObject;
	TScriptInterface<IWorldObjectPoolFactory> Factory;

	T* RequestInstance(InstanceParams p);
	void ReturnInstance(T*);

	void FlushPool();

protected:
	void IncreasePoolSize(int32 NewSize);
	void DecreasePoolSize(int32 NewSize);

};

UCLASS()
public UWorldObjectPoolSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
public:

};

*/
