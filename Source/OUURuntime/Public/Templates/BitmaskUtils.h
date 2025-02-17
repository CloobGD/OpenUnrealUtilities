// Copyright (c) 2021 Jonas Reich

#pragma once

#include "CoreMinimal.h"
#include "Traits/IsInteger.h"

/** How cases in the enum are distributed. */
enum class EEnumSequenceType
{
	/** Stub case used to check if the sequence trait was defined for an enum */
	Undefined,

	/**
	 * Cases used for Blueprint compatible bitmasks use natural numbers so a normal blueprint exposed uint8 enum
	 * can be used for 256 different cases instead of just 8 that would be possible with power-of-two cases.
	 * Sequence: 0, 1, 2, 3, 4, 5, 6, 7, 8, ...
	 */
	Natural,

	/**
	 * The more traditional C++ version uses power of two values (i.e. values in which only single bits are set)
	 * for the enum cases themselves so they can be more easily used in bitwise operations with the bitmask.
	 * This technique should only be used with third party libraries and code that is guaranteed to never be blueprint exposed.
	 * Sequence: 1, 2, 4, 8, 16, 32, 64, ...
	 */
	Pow2
};

/**
 * Trait template for enums that helps us distinguish statically what values the entries have, so the correct
 * function for bitmask conversion can be picked.
 */
template <class EnumClass>
struct TEnumSequenceTraits
{
	static const EEnumSequenceType Type = EEnumSequenceType::Undefined;
};

/** Macro to define enum sequence trait in a single line */
#define DECLARE_ENUM_SEQUENCE(EnumClass, SequenceType) \
	template<> struct TEnumSequenceTraits<EnumClass> \
	{ \
		static const EEnumSequenceType Type = SequenceType; \
	};


/**
 * Utility functions to deal with bitmask creation and evaluation.
 * 
 * All of the functions require the following template arguments:
 * - EnumType:
 *       The enum class to use as flags
 * - SequenceType (optional):
 *       How cases are distributed in the enum (see EEnumSequence declaration).
 *       This argument is optional: By default it's filled in from TEnumSequenceTraits.
 * - MaskIntegerType (optional):
 *       The integer type to use for the mask
 * 
 */
namespace BitmaskUtils
{
	template<class EnumType, EEnumSequenceType SequenceType = TEnumSequenceTraits<EnumType>::Type, typename MaskIntegerType = int32>
	constexpr MaskIntegerType EnumValueAsBitmask(EnumType EnumValue)
	{
		// This is the only function that really uses the SequenceType.
		// Because it's used everywhere else it's also sufficient to only have the static asserts in this single function.

		static_assert(TIsEnum<EnumType>::Value, "EnumType must be an enum or enum class");
		static_assert(TIsInteger<MaskIntegerType>::Value, "MaskIntegerType must be an integer type");
		static_assert(sizeof(MaskIntegerType) >= sizeof(EnumType) || SequenceType == EEnumSequenceType::Natural,
			"MaskIntegerType must be at least as big as underlying type of EnumType, so there is enough space for all flags");

		static_assert(SequenceType != EEnumSequenceType::Undefined, "SequenceType not defined for EnumType. "
			"You need to declare TEnumSequenceTraits specialization for EnumType so the correct bitmask conversion method can be determined.");

		const EEnumSequenceType TraitSequenceType = TEnumSequenceTraits<EnumType>::Type;
		static_assert(TraitSequenceType == EEnumSequenceType::Undefined || TraitSequenceType == SequenceType,
			"SequenceType is defined via TEnumSequenceTraits but also specified explicitly for BitmaskUtils template and the values mismatch! "
			"Please do not explicitly set the sequence type when invoking the template functions for enum types that have it defined via trait.");

		// Use static_cast instead of StaticCast, because the latter is not constexpr
		return SequenceType == EEnumSequenceType::Pow2 ? static_cast<MaskIntegerType>(EnumValue) : (1 << static_cast<MaskIntegerType>(EnumValue));
	}

	template<class EnumType, EEnumSequenceType SequenceType = TEnumSequenceTraits<EnumType>::Type, typename MaskIntegerType = int32>
	constexpr void SetBit(MaskIntegerType& BitmaskValue, EnumType EnumValue)
	{
		BitmaskValue |= EnumValueAsBitmask<EnumType, SequenceType, MaskIntegerType>(EnumValue);
	}

	template<class EnumType, EEnumSequenceType SequenceType = TEnumSequenceTraits<EnumType>::Type, typename MaskIntegerType = int32>
	constexpr void SetBits(MaskIntegerType& BitmaskValue, std::initializer_list<EnumType> EnumValues)
	{
		for (EnumType Flag : EnumValues)
		{
			SetBit<EnumType, SequenceType, MaskIntegerType>(BitmaskValue, Flag);
		}
	}

	template<class EnumType, EEnumSequenceType SequenceType = TEnumSequenceTraits<EnumType>::Type, typename MaskIntegerType = int32>
	constexpr MaskIntegerType EnumValuesAsBitmask(std::initializer_list<EnumType> EnumValues)
	{
		MaskIntegerType BitmaskValue = 0;
		SetBits<EnumType, SequenceType, MaskIntegerType>(BitmaskValue, EnumValues);
		return BitmaskValue;
	}

	template<class EnumType, EEnumSequenceType SequenceType = TEnumSequenceTraits<EnumType>::Type, typename MaskIntegerType = int32>
	constexpr void ClearBit(MaskIntegerType& BitmaskValue, EnumType EnumValue)
	{
		BitmaskValue &= ~EnumValueAsBitmask<EnumType, SequenceType, MaskIntegerType>(EnumValue);
	}

	template<class EnumType, EEnumSequenceType SequenceType = TEnumSequenceTraits<EnumType>::Type, typename MaskIntegerType = int32>
	constexpr void ClearBits(MaskIntegerType& BitmaskValue, std::initializer_list<EnumType> EnumFlags)
	{
		for (EnumType Flag : EnumFlags)
		{
			BitmaskValue &= ~EnumValueAsBitmask<EnumType, SequenceType, MaskIntegerType>(Flag);
		}
	}

	template<class EnumType, EEnumSequenceType SequenceType = TEnumSequenceTraits<EnumType>::Type, typename MaskIntegerType = int32>
	constexpr void SetBit(MaskIntegerType& BitmaskValue, EnumType EnumValue, bool bBitValue)
	{
		if (bBitValue)
		{
			SetBit(BitmaskValue, EnumValue);
		}
		else
		{
			ClearBit(BitmaskValue, EnumValue);
		}
	}

	template<class EnumType, EEnumSequenceType SequenceType = TEnumSequenceTraits<EnumType>::Type, typename MaskIntegerType = int32>
	constexpr void SetBits(MaskIntegerType& BitmaskValue, std::initializer_list<EnumType> EnumValues, bool bBitValue)
	{
		if (bBitValue)
		{
			SetBits(BitmaskValue, EnumValues);
		}
		else
		{
			ClearBits(BitmaskValue, EnumValues);
		}
	}

	template<class EnumType, EEnumSequenceType SequenceType = TEnumSequenceTraits<EnumType>::Type, typename MaskIntegerType = int32>
	constexpr bool TestBit(MaskIntegerType BitmaskValue, EnumType EnumValue)
	{
		return (BitmaskValue & EnumValueAsBitmask<EnumType, SequenceType, MaskIntegerType>(EnumValue)) > 0;
	};

	template<class EnumType, EEnumSequenceType SequenceType = TEnumSequenceTraits<EnumType>::Type, typename MaskIntegerType = int32>
	constexpr bool TestAllBits(MaskIntegerType BitmaskValue, std::initializer_list<EnumType> EnumFlags)
	{
		MaskIntegerType FlagsValue = EnumValuesAsBitmask<EnumType, SequenceType, MaskIntegerType>(EnumFlags);
		return (BitmaskValue & FlagsValue) == FlagsValue;
	};

	template<class EnumType, EEnumSequenceType SequenceType = TEnumSequenceTraits<EnumType>::Type, typename MaskIntegerType = int32>
	constexpr bool TestAnyBits(MaskIntegerType BitmaskValue, std::initializer_list<EnumType> EnumFlags)
	{
		MaskIntegerType FlagsValue = EnumValuesAsBitmask<EnumType, SequenceType, MaskIntegerType>(EnumFlags);
		return (BitmaskValue & FlagsValue) > 0;
	};
};
