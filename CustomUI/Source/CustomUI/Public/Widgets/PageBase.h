// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WidgetBase.h"
#include "PageBase.generated.h"

/**
 * viewport에 오직 하나만 존재 한다.
 */
UCLASS(Abstract)
class CUSTOMUI_API UPageBase : public UWidgetBase
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere)
	FPageConfig _PageConfig;

public:
	UFUNCTION(BlueprintPure)
	const FPageConfig& GetConfig() const { return _PageConfig; }

};
