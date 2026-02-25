// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "StringTableSubsystem.generated.h"

/*
 * 
 */
UCLASS()
class CUSTOMUI_API UStringTableSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

protected:
	// <table name, string table id>
	static TMap<FString, FName> _NameIDMap;

public:
	virtual void Initialize(FSubsystemCollectionBase& _collection) override;

public:
	static FText FindTextFromTable(const FString& _table_name, const FString& _key);
};
