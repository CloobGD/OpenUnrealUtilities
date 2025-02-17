﻿// Copyright (c) 2021 Jonas Reich

#pragma once

#include "AbilitySystemComponent.h"
#include "Containers/CircularQueue.h"
#include "Templates/CircularArrayAdaptor.h"

#include "OUUAbilitySystemComponent.generated.h"

/**
 * Utility structure used to copy data from FGameplayEventData for long time storage in an event history.
 * The original structure from the engine is very likely to cause crashes during copy assignment.
 */
USTRUCT(BlueprintType)
struct OUURUNTIME_API FOUUGameplayEventData
{
	GENERATED_BODY()
public:
	FOUUGameplayEventData() = default;
	FOUUGameplayEventData(const FGameplayEventData& SourcePayload) :
		Timestamp(FDateTime::Now()),
		EventTag(SourcePayload.EventTag),
		Instigator(SourcePayload.Instigator),
		Target(SourcePayload.Target),
		EventMagnitude(SourcePayload.EventMagnitude)
	{
	}

	/** When the event occured (compare with FDateTime::Now()) */
	UPROPERTY()
	FDateTime Timestamp = FDateTime::MinValue();

	/** Tag of the event that triggered this */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GameplayAbilityTriggerPayload)
	FGameplayTag EventTag = {};

	/** The instigator of the event */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GameplayAbilityTriggerPayload)
	const AActor* Instigator = nullptr;

	/** The target of the event */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GameplayAbilityTriggerPayload)
	const AActor* Target = nullptr;

	/** The magnitude of the triggering event */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GameplayAbilityTriggerPayload)
	float EventMagnitude = 0.f;
};

/**
 * Custom ability system component that provides friend access to FGameplayDebuggerCategory_OUUAbilities
 * (required to access some of the protected members of the parent class)
 * and record additional debugging data, e.g. history of gameplay events. 
 */
UCLASS()
class OUURUNTIME_API UOUUAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
private:
	friend class FGameplayDebuggerCategory_OUUAbilities;

public:
	// - UAbilitySystemComponent
	virtual int32 HandleGameplayEvent(FGameplayTag EventTag, const FGameplayEventData* Payload) override;
	// --

protected:
	/**
	 * History of all gameplay events encountered via HandleGameplayEvent()
	 * Intended for debugging purposes.
	 */
	UPROPERTY()
	TArray<FOUUGameplayEventData> GameplayEventHistory;

	/**
	 * Circular buffer adapter for the gameplay event history.
	 * Use this to access the history elements!
	 */
	TCircularArrayAdaptor<FOUUGameplayEventData> CircularGameplayEventHistory {GameplayEventHistory, 10};
};

