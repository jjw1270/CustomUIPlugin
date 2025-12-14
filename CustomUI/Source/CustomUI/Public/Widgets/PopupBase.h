// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WidgetBase.h"
#include "PopupBase.generated.h"



/**
 * viewport에 중복으로 존재 가능하다.
 */
UCLASS(Abstract)
class CUSTOMUI_API UPopupBase : public UWidgetBase
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere)
	FPopupConfig _PopupConfig;

public:
	UFUNCTION(BlueprintPure)
	const FPopupConfig& GetConfig() const { return _PopupConfig; }
};
