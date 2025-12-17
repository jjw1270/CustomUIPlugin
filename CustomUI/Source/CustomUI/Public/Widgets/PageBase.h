// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WidgetBase.h"
#include "PageBase.generated.h"

/**
 * viewport에 하나만 존재.
 */
UCLASS(Abstract, HideDropdown)
class CUSTOMUI_API UPageBase : public UWidgetBase
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditDefaultsOnly)
	FPageConfig _PageConfig;

protected:
	virtual void NativeConstruct() override;

public:
	UFUNCTION(BlueprintPure)
	const FPageConfig& GetConfig() const { return _PageConfig; }

};
