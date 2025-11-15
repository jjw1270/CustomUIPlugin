// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ButtonBase.h"
#include "TabButton.generated.h"

USTRUCT(BlueprintType)
struct FTabButtonConfig
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Button")
	TMap<EButtonState, FSlateBrush> ButtonStyle;

	UPROPERTY(EditAnywhere, Category = "Sound")
	TObjectPtr<USoundCue> HoverSound = nullptr;

	UPROPERTY(EditAnywhere, Category = "Sound")
	TObjectPtr<USoundCue> ClickSound = nullptr;
};

UCLASS(Abstract)
class CUSTOMUI_API UTabButton : public UButtonBase
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(BlueprintReadOnly)
	FName _TabID;

protected:
	virtual void SynchronizeProperties() override;

public:
	void InitWidget(FName _tab_id, const FTabButtonConfig& _config);

public:
	UFUNCTION(BlueprintPure)
	FName GetTabID() const { return _TabID; }
};
