// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WidgetBase.h"
#include "PopupBase.generated.h"

/**
 * 
 */
UCLASS(Abstract, HideDropdown)
class CUSTOMUI_API UPopupBase : public UWidgetBase
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditDefaultsOnly)
	FPopupConfig _PopupConfig;

protected:
	virtual void NativeConstruct() override;

public:
	UFUNCTION(BlueprintPure)
	const FPopupConfig& GetConfig() const { return _PopupConfig; }
};
