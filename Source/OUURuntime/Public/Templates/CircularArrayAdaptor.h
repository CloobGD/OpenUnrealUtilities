﻿// Copyright (c) 2021 Jonas Reich

#pragma once

#include "ReferenceWrapper.h"
#include "Containers/Array.h"

template <class ChildClass, typename ElementType, typename AllocatorType>
class TCircularArrayAdaptor_Base
{
public:
	using SizeType = typename AllocatorType::SizeType;
	using ArrayType = TArray<ElementType, AllocatorType>;

	TCircularArrayAdaptor_Base(ArrayType& InArrayReference, SizeType InArrayMax) :
		StorageReference(InArrayReference),
		WriteIndex(0),
		ArrayMax(InArrayMax)
	{
	}

	TCircularArrayAdaptor_Base(const TCircularArrayAdaptor_Base& Other) :
		StorageReference(Other.StorageReference),
		WriteIndex(Other.WriteIndex),
		ArrayMax(Other.ArrayMax)
	{
		*this = Other;
	}

	TCircularArrayAdaptor_Base(TCircularArrayAdaptor_Base&&) = delete;

	TCircularArrayAdaptor_Base& operator=(const TCircularArrayAdaptor_Base& Other)
	{
		StorageReference = Other.StorageReference;
		WriteIndex = Other.WriteIndex;
		ArrayMax = Other.ArrayMax;

		return *this;
	}

	TCircularArrayAdaptor_Base& operator==(TCircularArrayAdaptor_Base&&) = delete;

	void Add(ElementType Element)
	{
		if (IsPreWrap())
		{
			GetStorage().AddUninitialized(1);
		}
		ArrayType& ArrayReference = GetStorage();
		check(ArrayReference.IsValidIndex(WriteIndex));
		ArrayReference[WriteIndex] = Element;
		WriteIndex = (WriteIndex + 1) % ArrayMax;
	}

	SizeType Num() const
	{
		return GetStorage().Num();
	}

	bool HasData() const
	{
		return Num() > 0;
	}

	ElementType Last() const
	{
		const int32 LastIdx = WriteIndex - 1;
		const int32 LastIdxWrapped = LastIdx >= 0 ? LastIdx : LastIdx + Num();
		return GetStorage()[LastIdxWrapped];
	}

	ElementType Oldest() const
	{
		if (IsPreWrap())
		{
			return GetStorage()[0];
		}
		return GetStorage()[WriteIndex];
	}

	ArrayType& GetStorage()
	{
		return StorageReference.Get();
	}

	const ArrayType& GetStorage() const
	{
		return StorageReference.Get();
	}

	bool IsValidIndex(SizeType Index)
	{
		return GetStorage().IsValidIndex(GetWrappedRingIndex(Index));
	}

	ElementType& operator[](SizeType Index)
	{
		return GetStorage()[GetWrappedRingIndex(Index)];
	}

	// Iterators
	using TIterator = TIndexedContainerIterator<TCircularArrayAdaptor_Base, ElementType, SizeType>;
	using TConstIterator = TIndexedContainerIterator<const TCircularArrayAdaptor_Base, const ElementType, SizeType>;

	FORCEINLINE TIterator begin() { return TIterator(*this, 0); }
	FORCEINLINE TConstIterator begin() const { return TConstIterator(*this, 0); }
	FORCEINLINE TIterator end() { return TIterator(*this, Num()); }
	FORCEINLINE TConstIterator end() const { return TConstIterator(*this, Num()); }

protected:
	TReferenceWrapper<ArrayType> StorageReference;
	int32 WriteIndex = 0;
	SizeType ArrayMax = 0;

	bool IsPreWrap() const
	{
		return Num() < ArrayMax;
	}

	int32 GetWrappedRingIndex(int32 Index) const
	{
		checkf(GetStorage().IsValidIndex(Index), TEXT("%i is an invalid index for storage with size %i. "
			"You must stick to indices >= 0 and < Num just like with regular arrays!"), Index, Num());
		const int32 RingIndex = (WriteIndex + Index);
		const int32 WrappedRingIndex = (RingIndex >= Num()) ? (RingIndex - Num()) : RingIndex;
		return WrappedRingIndex;
	}
};

template <typename ElementType, typename AllocatorType = FDefaultAllocator>
class TCircularArrayAdaptor : public TCircularArrayAdaptor_Base<TCircularArrayAdaptor<ElementType, AllocatorType>, ElementType, FDefaultAllocator>
{
public:
	using SelfType = TCircularArrayAdaptor<ElementType, AllocatorType>;
	using Super = TCircularArrayAdaptor_Base<SelfType, ElementType, FDefaultAllocator>;
	using ArrayType = typename Super::ArrayType;

	TCircularArrayAdaptor(ArrayType& InArrayReference, int32 InArrayMax) :
		Super(InArrayReference, InArrayMax)
	{
	}
};
