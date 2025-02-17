﻿// Copyright (c) 2021 Jonas Reich

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "OUULexicalOperatorLibrary.generated.h"

/**
 * Comparison operators for lexical core types (FString, FName).
 */
UCLASS()
class OUUBLUEPRINTRUNTIME_API UOUULexicalOperatorLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	// FString

	/** Returns true if A is lexically less than B (A < B) */
	UFUNCTION(BlueprintPure, meta=(DisplayName = "String < String", CompactNodeTitle = "<", Keywords = "< less"), Category = "Open Unreal Utilities|Operators")
	static bool Less_StringString(const FString& A, const FString& B);

	/** Returns true if A is lexically greater than B (A > B) */
	UFUNCTION(BlueprintPure, meta=(DisplayName = "String > String", CompactNodeTitle = ">", Keywords = "> greater"), Category = "Open Unreal Utilities|Operators")
	static bool Greater_StringString(const FString& A, const FString& B);

	/** Returns true if A is lexically less than or equal to B (A <= B) */
	UFUNCTION(BlueprintPure, meta=(DisplayName = "String <= String", CompactNodeTitle = "<=", Keywords = "<= less"), Category="Open Unreal Utilities|Operators")
	static bool LessEqual_StringString(const FString& A, const FString& B);

	/** Returns true if A is lexically greater than or equal to B (A >= B) */
	UFUNCTION(BlueprintPure, meta=(DisplayName = "String >= String", CompactNodeTitle = ">=", Keywords = ">= greater"), Category="Open Unreal Utilities|Operators")
	static bool GreaterEqual_StringString(const FString& A, const FString& B);

	// FName

	/** Returns true if A is lexically less than B (A < B) */
	UFUNCTION(BlueprintPure, meta=(DisplayName = "Name < Name", CompactNodeTitle = "<", Keywords = "< less"), Category = "Open Unreal Utilities|Operators")
	static bool Less_NameName(const FName& A, const FName& B);

	/** Returns true if A is lexically greater than B (A > B) */
	UFUNCTION(BlueprintPure, meta=(DisplayName = "Name > Name", CompactNodeTitle = ">", Keywords = "> greater"), Category = "Open Unreal Utilities|Operators")
	static bool Greater_NameName(const FName& A, const FName& B);

	/** Returns true if A is lexically less than or equal to B (A <= B) */
	UFUNCTION(BlueprintPure, meta=(DisplayName = "Name <= Name", CompactNodeTitle = "<=", Keywords = "<= less"), Category="Open Unreal Utilities|Operators")
	static bool LessEqual_NameName(const FName& A, const FName& B);

	/** Returns true if A is lexically greater than or equal to B (A >= B) */
	UFUNCTION(BlueprintPure, meta=(DisplayName = "Name >= Name", CompactNodeTitle = ">=", Keywords = ">= greater"), Category="Open Unreal Utilities|Operators")
	static bool GreaterEqual_NameName(const FName& A, const FName& B);
};
