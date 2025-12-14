// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WidgetDefines.generated.h"


UENUM(BlueprintType)
enum class EInputMode : uint8
{
	GameAndUI,
	GameOnly,
	UIOnly,
};

USTRUCT(BlueprintType)
struct FPageConfig
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category="InputMode")
	EInputMode InputMode = EInputMode::GameAndUI;

	UPROPERTY(EditAnywhere, Category = "InputMode", meta = (EditCondition = "InputMode==EInputMode::UIOnly || InputMode==EInputMode::GameAndUI"))
	bool SetFocus = true;

	UPROPERTY(EditAnywhere, Category = "InputMode", meta = (EditCondition = "InputMode==EInputMode::UIOnly || InputMode==EInputMode::GameAndUI"))
	EMouseLockMode InMouseLockMode = EMouseLockMode::DoNotLock;

	UPROPERTY(EditAnywhere, Category = "InputMode", meta = (EditCondition = "InputMode==EInputMode::GameAndUI"))
	bool HideCursorDuringCapture = true;

	UPROPERTY(EditAnywhere, Category = "InputMode")
	bool FlushInput = false;

	UPROPERTY(EditAnywhere)
	bool ShowMouseCursor = true;

	UPROPERTY(EditAnywhere, meta=(Tooltip="Remain On Level Changed"))
	bool RemainOnLevelChanged = false;


};

UENUM(BlueprintType)
enum class EPopupStackPriority : uint8
{
	Page = 0 UMETA(Hidden),

};

USTRUCT(BlueprintType)
struct FPopupConfig
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	EPopupStackPriority StackPriority = EPopupStackPriority::Page;

	UPROPERTY(EditAnywhere)
	bool ShowMouseCursor = true;

	UPROPERTY(EditAnywhere)
	bool RemainOnLevelChanged = false;

};
